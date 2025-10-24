#include "stdio.h"
#include "string.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_sai.h"
#include "stm32f7xx_hal_dma.h"
#include "i2c.h"
#include "tim.h"
#include "math.h"
#include "main.h"
#include "PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.h"
#include "JSB_ILI9341.h"
#include "JSB_XPT2046.h"
#include "gfxfont.h"
#include "FreeSans9pt7b.h"
#include "FreeSans12pt7b.h"
#include "go.h"

///////////////////////////////////////////////////////////////////////////////

int wrap(int Value, int Size)
{
  if (Value < 0)
    Value += Size;
  else if (Value >= Size)
    Value -= Size;

  return Value;
}

///////////////////////////////////////////////////////////////////////////////

#define CalculateFrequency_NumSamples (4096)
#define CalculateFrequency_NumReadSamples (10)
#define CalculateFrequency_MinFrequency_StopThreshold (10.0f)
#define CalculateFrequency_MinFrequency_StartThreshold (12.0f)

static uint32_t CalculateFrequency_Samples[CalculateFrequency_NumSamples];
static uint8_t InputStopped = 0;
static float CalculateFrequency_TimerCountFrequency = 0.0f;

void CalculateFrequency_Reset()
{
}

uint8_t CalculateFrequency(float *o_pFrequency)
{
  uint32_t ReadIndex, WriteIndex;
  uint32_t AverageTimePerCycle_InCounterCycles;
  uint32_t CurrentTimerCount, CurrentSampleCount, NumCountsSinceLastSample;
  float Frequency, TimeSinceLastSample;

  *o_pFrequency = 0.0f;

  WriteIndex = CalculateFrequency_NumSamples - htim2.hdma[TIM_DMA_ID_CC1]->Instance->NDTR;
  if (WriteIndex >= CalculateFrequency_NumSamples)
    return 0;
  ReadIndex = wrap(WriteIndex - 1, CalculateFrequency_NumSamples);

  CurrentTimerCount = __HAL_TIM_GetCounter(&htim2);
  CurrentSampleCount = CalculateFrequency_Samples[ReadIndex];

  NumCountsSinceLastSample = CurrentTimerCount - CurrentSampleCount;
  TimeSinceLastSample = (float)NumCountsSinceLastSample / (float)CalculateFrequency_TimerCountFrequency;

  AverageTimePerCycle_InCounterCycles = (CurrentSampleCount - CalculateFrequency_Samples[wrap(ReadIndex - CalculateFrequency_NumReadSamples, CalculateFrequency_NumSamples)]) / CalculateFrequency_NumReadSamples;
  Frequency = (float)CalculateFrequency_TimerCountFrequency / (float)AverageTimePerCycle_InCounterCycles;

  if (InputStopped)
  {
    if (Frequency >= CalculateFrequency_MinFrequency_StartThreshold)
      InputStopped = 0;
  }
  else
  {
    if ((Frequency < CalculateFrequency_MinFrequency_StopThreshold) || (TimeSinceLastSample >= 1.0f / CalculateFrequency_MinFrequency_StopThreshold))
      InputStopped = 1;
  }

  if (InputStopped)
    Frequency = 0.0f;

  *o_pFrequency = Frequency;

  return 1;
}

///////////////////////////////////////////////////////////////////////////////

void Go()
{
  float InputFrequency;
  char S[200];

  CalculateFrequency_TimerCountFrequency = (float)GetAPB1TimerClockFrequency() / ((float)htim2.Instance->PSC + 1.0f);

  ILI9341_SetFont(&FreeSans9pt7b);
  ILI9341_SetTextColor(ILI9341_COLOR_WHITE);
  ILI9341_SetTextBackgroundColor(ILI9341_COLOR_BLACK);
  ILI9341_SetTextDrawMode(tdmAnyCharBar); // Slower but enables flicker free update.

  ILI9341_Clear(0x0000);

  ILI9341_DrawTextAtXY("Frequency meter", 0, ILI9341_GetFontYSpacing(), tpLeft);
  ILI9341_DrawTextAtXY("Input to TIM2_CH1 (pin 77)", 0, 2 * ILI9341_GetFontYSpacing(), tpLeft);

  if (HAL_TIM_Base_Start(&htim2))
    Error_Handler();
  if (HAL_TIM_IC_Start_DMA(&htim2, TIM_CHANNEL_1, CalculateFrequency_Samples, CalculateFrequency_NumSamples) != HAL_OK)
    Error_Handler();

  CalculateFrequency_Reset();

  while(1)
  {
    if (CalculateFrequency(&InputFrequency))
    {
      sprintf(S, "Input frequency: %0.2f             ", InputFrequency);
      ILI9341_DrawTextAtXY(S, 0, 3 * ILI9341_GetFontYSpacing(), tpLeft);
    }

    HAL_Delay(1); //!!! 100
  }
}

