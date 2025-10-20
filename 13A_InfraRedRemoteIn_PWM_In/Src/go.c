#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_sai.h"
#include "stm32f7xx_hal_dma.h"
#include "tim.h"
#include "i2c.h"
#include "spi.h"
#include "sai.h"
#include "main.h"
#include "PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.h"
#include "JSB_General.h"
#include "JSB_UI_Elements.h"
#include "go.h"

//////////////////////////////////////////////////////////////////////////////

#define MaxNumCaptureInfos 100

static uint32_t NumCaptureInfos = 0;
static uint8_t Stop = 0;

typedef struct
{
  uint32_t Ch1, Ch2;
} CaptureInfo_t;

CaptureInfo_t CaptureInfos[MaxNumCaptureInfos];

//////////////////////////////////////////////////////////////////////////////

static uint32_t CurrentTimerCount = 0; // !!!

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim3)
  {
//    ToggleLED(LED_Blue);
    HAL_GPIO_WritePin(TestPoint_GPIO_Port, TestPoint_Pin, 1);
    if (IsUserButtonPressed())
      CurrentTimerCount = __HAL_TIM_GetCounter(&htim3);
    HAL_GPIO_WritePin(TestPoint_GPIO_Port, TestPoint_Pin, 0);
  }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim3)
  {
//    ToggleLED(LED_Blue);

    if (Stop)
      return;

    if (NumCaptureInfos != MaxNumCaptureInfos)
    {
      CaptureInfo_t CaptureInfo;

      CaptureInfo.Ch1 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);
      CaptureInfo.Ch2 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2);
      CaptureInfos[NumCaptureInfos] =CaptureInfo;
      ++NumCaptureInfos;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

void Go()
{
  JSB_UI_InitializeDisplay();

  // float TimerCountFrequency = (float)GetAPB1TimerClockFrequency() / ((float)htim3.Instance->PSC + 1.0f);

  if (HAL_TIM_Base_Start_IT(&htim3))
    Error_Handler();
  if (HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1))
    Error_Handler();
  if (HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2))
    Error_Handler();

  __HAL_TIM_URS_ENABLE(&htim3); // Prevent HAL_TIM_PeriodElapsedCallback() being called when counter is reset by input.

  while (1)
  {
    char S[256];

    ILI9341_Clear(0);

//    sprintf(S, "Receive IR:");
//    ILI9341_DrawTextAtXY(S, 0, 1 * ILI9341_GetFontYSpacing(), tpLeft);
//    sprintf(S, "Input to Tim3-Ch1 (pin 63):");
//    ILI9341_DrawTextAtXY(S, 0, 2 * ILI9341_GetFontYSpacing(), tpLeft);

    sprintf(S, "Timer count: %lu", CurrentTimerCount);
    ILI9341_DrawTextAtXY(S, 0, 1 * (ILI9341_GetFontYSpacing() - 4), tpLeft);

//    sprintf(S, "Tim3 Ch1: %lu             ", HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1));
//    ILI9341_DrawTextAtXY(S, 0, 4 * ILI9341_GetFontYSpacing(), tpLeft);
//    sprintf(S, "Tim3 Ch2: %lu             ", HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2));
//    ILI9341_DrawTextAtXY(S, 0, 5 * ILI9341_GetFontYSpacing(), tpLeft);
//
//    sprintf(S, "NumValues: %lu             ", NumValues);
//    ILI9341_DrawTextAtXY(S, 0, 6 * ILI9341_GetFontYSpacing(), tpLeft);

//    if (NumCaptureInfos == MaxNumCaptureInfos)
//    {
//      uint32_t BitIndex;
//      uint32_t CaptureInfoIndex = 1;
//      uint32_t DecodedBitIndex = 0;
//      uint8_t DecodedBits[128];
//
//      do
//      {
//        CaptureInfo_t CaptureInfo = CaptureInfos[CaptureInfoIndex];
//        for (BitIndex = 0; BitIndex < (int)round(CaptureInfo.Ch2 / 889.0f); ++BitIndex)
//          DecodedBits[DecodedBitIndex++] = 1;
//        for (BitIndex = 0; BitIndex < (int)round((CaptureInfo.Ch1 - CaptureInfo.Ch2) / 889.0f); ++BitIndex)
//          DecodedBits[DecodedBitIndex++] = 0;
//        ++CaptureInfoIndex;
//      } while(CaptureInfoIndex < MaxNumCaptureInfos); //!!!
//      uint32_t NumDecodedBits = DecodedBitIndex;
//
//      for (int DecodedBitIndex = 0; DecodedBitIndex < NumDecodedBits; DecodedBitIndex += 2)
//      {
//        sprintf(S, "%d %d", DecodedBits[DecodedBitIndex], DecodedBits[DecodedBitIndex + 1]);
//        ILI9341_DrawTextAtXY(S, 0, ((DecodedBitIndex >> 1) + 2) * (ILI9341_GetFontYSpacing() - 4), tpLeft);
//      }

//      for (int ItemIndex = 0; ItemIndex < 12; ++ItemIndex)
//      {
//        sprintf(S, "%04d %04d", (int)(round(CaptureInfos[ItemIndex].Ch1 / 889.0f)), (int)(round(CaptureInfos[ItemIndex].Ch2 / 889.0f)));
//        ILI9341_DrawTextAtXY(S, 0, (ItemIndex + 3) * ILI9341_GetFontYSpacing(), tpLeft);
//      }

//      HAL_Delay(1000);
//      NumCaptureInfos = 0;
      // Stop = 0;
//    }

    if (ILI9341_UsingBackBuffer())
      ILI9341_CopyBackBufferToDisplay();
  }
}
