#include "string.h"
#include "math.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_sai.h"
#include "stm32f7xx_hal_dma.h"
#include "sai.h"
#include "main.h"
#include "PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.h"
#include "JSB_ILI9341.h"
#include "JSB_XPT2046.h"
#include "gfxfont.h"
#include "FreeSans9pt7b.h"
#include "FreeSans12pt7b.h"
#include "go.h"

#define AudioData_SampleFrequency 48000
#define AudioData_NumSamples 32000 /* Beware: DMA transfers are limited to 65535 items. We transfer two items per sample. Hence 32767 samples max.  */
#define AudioData_NumFadeSamples 100
#define AudioData_GeneratedFrequency 1000.0

typedef struct
{
  int32_t Left, Right;
} AudioData_Sample_t;

AudioData_Sample_t AudioData[AudioData_NumSamples];

void GenerateWaveform()
{
  double SampleValue_float, FadeScaleFactor;
  int32_t SampleValue_int;
  AudioData_Sample_t *pSample;

  for (int SampleIndex = 0; SampleIndex < AudioData_NumSamples; ++SampleIndex)
  {
    FadeScaleFactor = 1.0;
    if (SampleIndex < AudioData_NumFadeSamples)
      FadeScaleFactor = (double)SampleIndex / (double)AudioData_NumFadeSamples;
    else if (SampleIndex > (AudioData_NumSamples - AudioData_NumFadeSamples))
        FadeScaleFactor = (double)(AudioData_NumSamples - SampleIndex) / (double)AudioData_NumFadeSamples;

    SampleValue_float = FadeScaleFactor * sin((double)AudioData_GeneratedFrequency * 2.0 * M_PI * ((double)SampleIndex / (double)AudioData_SampleFrequency));
    SampleValue_int = (int32_t)(65536 * 32767 * SampleValue_float);

    pSample = &AudioData[SampleIndex];
    //
    pSample->Left = SampleValue_int;
    pSample->Right = SampleValue_int;
  }
}

void Go()
{
  uint16_t Y = 0;
  
  ILI9341_SetFont(&FreeSans12pt7b);
  ILI9341_SetTextColor(ILI9341_COLOR_WHITE);
  ILI9341_SetTextBackgroundColor(ILI9341_COLOR_BLACK);
  ILI9341_SetTextDrawMode(tdmAnyCharBar); // Slower but enables flicker free update.

  ILI9341_Clear(0x0000);

  Y = 0;

  Y += ILI9341_GetFontYSpacing();
  ILI9341_DrawTextAtXY("Tone=>DAC", 0, Y, tpLeft);

  DAC_Initialize();

  GenerateWaveform();

  DAC_Mute(0);

  while(1)
  {
    ToggleLED(LED_Green);

	// Non-DMA version:
//    if (HAL_SAI_Transmit(&hsai_BlockA2, (uint8_t *)AudioData, 2 * AudioData_NumSamples, 5000) != HAL_OK)
//      HAL_GPIO_TogglePin(LED_Red_GPIO_Port, LED_Red_Pin);

    SCB_CleanDCache_by_Addr((uint32_t *)AudioData, AudioData_NumSamples * sizeof(AudioData_Sample_t));
    HAL_StatusTypeDef HAL_Result = HAL_SAI_Transmit_DMA(&hsai_BlockA2, (uint8_t *)AudioData, (uint16_t)((sizeof(AudioData_Sample_t) / sizeof(int32_t)) * AudioData_NumSamples));
    if (HAL_Result != HAL_OK)
      ToggleLED(LED_Red);

    HAL_Delay(1200);
  }
}
