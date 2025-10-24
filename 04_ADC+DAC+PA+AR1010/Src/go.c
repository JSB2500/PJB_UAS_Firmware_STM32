#include <stdio.h>
#include "math.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_sai.h"
#include "stm32f7xx_hal_dma.h"
#include "i2c.h"
#include "sai.h"
#include "main.h"
#include "PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.h"
#include "JSB_ILI9341.h"
#include "JSB_XPT2046.h"
#include "AR1010.h"
#include "gfxfont.h"
#include "FreeSans9pt7b.h"
#include "FreeSans12pt7b.h"
#include "JSB_General.h"
#include "go.h"

///////////////////////////////////////////////////////////////////////////////

float AudioTransferGain = 1.0f;
float PA_MasterVolume_dB = -20.0f;

typedef struct
{
  int32_t Left, Right;
} AudioData_Sample_t;

#define AudioData_NumChannels 2

#define AudioBuffer_NumSamplesPerHalf 16
#define AudioBuffer_NumSamplesTotal (2 * AudioBuffer_NumSamplesPerHalf)

AudioData_Sample_t AudioInputBuffer[AudioBuffer_NumSamplesTotal];
AudioData_Sample_t AudioOutputBuffer[AudioBuffer_NumSamplesTotal];

#define UseSingleBuffer 0

///////////////////////////////////////////////////////////////////////////////

float I2S_24bitToNormalizedFloat(int32_t Value)
{
  if (Value & 0x00800000)
    Value |= 0xFF000000;

  return Value / 16777216.0f;
}

int32_t I2S_NormalizedFloatTo24bit(float Value)
{
  return round(16777216.0f * Value);
}

void AudioBuffer_TransferFromInputBufferToOutputBuffer(AudioBufferHalf_t AudioBufferHalf)
{
  uint16_t FirstSampleIndex, SampleIndex;
  float Audio_Left, Audio_Right;
  AudioData_Sample_t *pInputSample, *pOutputSample;

  FirstSampleIndex = 0;
  switch (AudioBufferHalf)
  {
    case abhFirstHalf: FirstSampleIndex = 0; break;
    case abhSecondHalf: FirstSampleIndex = AudioBuffer_NumSamplesPerHalf; break;
    default: ToggleLED(LED_Red); //!!!
  }

  for(SampleIndex = FirstSampleIndex; SampleIndex < FirstSampleIndex + AudioBuffer_NumSamplesPerHalf; ++SampleIndex)
  {
    pInputSample = &AudioInputBuffer[SampleIndex];
    pOutputSample = &AudioOutputBuffer[SampleIndex];

    Audio_Left = I2S_24bitToNormalizedFloat(pInputSample->Left);
    Audio_Right = I2S_24bitToNormalizedFloat(pInputSample->Right);

    Audio_Left *= AudioTransferGain;
    Audio_Right *= AudioTransferGain;

    pOutputSample-> Left = I2S_NormalizedFloatTo24bit(Audio_Left);
    pOutputSample-> Right = I2S_NormalizedFloatTo24bit(Audio_Right);
  }
}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (UseSingleBuffer)
    return;

  // For debugging Rx/Tx timing.
  HAL_GPIO_WritePin(SAI_RxDMA_GPIO_Port, SAI_RxDMA_Pin, 1);

  AudioBuffer_TransferFromInputBufferToOutputBuffer(abhFirstHalf);
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (UseSingleBuffer)
    return;

  // For debugging Rx/Tx timing.
  HAL_GPIO_WritePin(SAI_RxDMA_GPIO_Port, SAI_RxDMA_Pin, 0);

  AudioBuffer_TransferFromInputBufferToOutputBuffer(abhSecondHalf);
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (UseSingleBuffer)
    return;

  // For debugging Rx/Tx timing.
  HAL_GPIO_WritePin(SAI_TxDMA_GPIO_Port, SAI_TxDMA_Pin, 1);
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (UseSingleBuffer)
    return;

  // For debugging Rx/Tx timing.
  HAL_GPIO_WritePin(SAI_TxDMA_GPIO_Port, SAI_TxDMA_Pin, 0);
}

///////////////////////////////////////////////////////////////////////////////

void Go()
{
  ADC_Source_t ADC_Source;
  uint16_t Y = 0;
  int16_t Touch_RawX, Touch_RawY, Touch_RawZ;
  int16_t Touch_ScreenX, Touch_ScreenY;
  uint8_t PA_ErrorStatus;
  char S[128];
  
  ILI9341_SetFont(&FreeSans12pt7b);
  ILI9341_SetTextColor(ILI9341_COLOR_WHITE);
  ILI9341_SetTextBackgroundColor(ILI9341_COLOR_BLACK);
  ILI9341_SetTextDrawMode(tdmAnyCharBar); // Slower but enables flicker free update.

  ILI9341_Clear(0x0000);

  uint8_t UserButtonPressed = 0;

  ESP32_B_Enable(1); // For I2S MCLK. (ESP32 must be programmed accordingly).
  //
  I2S_SelectSource(I2S_Source_ADC);
  //
  ADC_Initialize();
  //
  DAC_Initialize();
  //
  AR1010_init();
  AR1010_auto_tune(92.1, 1); // JSB: Was 96.4. Sheffield: 88.6 for Radio Sheffield. 92.1 for Radio 3.
  AR1010_set_volume(18);
  //
  PA_Initialize(0);
  //PA_Initialize(1); // Headphones.
  PA_SetMasterVolume(PA_MasterVolume_dB);

  ADC_Source = ADC_Source_BlueJack;
  ADC_SelectSource(ADC_Source);

  Y += ILI9341_GetFontYSpacing();
  ILI9341_DrawTextAtXY("ADC=>DAC+PA", 0, Y, tpLeft);

  Y += ILI9341_GetFontYSpacing();
  if (UseSingleBuffer)
    ILI9341_DrawTextAtXY("Using single buffer", 0, Y, tpLeft);
  else
    ILI9341_DrawTextAtXY("Using I&O buffers", 0, Y, tpLeft);

  if (UseSingleBuffer)
  {
    HandleHALResult(HAL_SAI_Receive_DMA(&hsai_BlockB2, (uint8_t *)AudioInputBuffer, (uint16_t)(AudioData_NumChannels * AudioBuffer_NumSamplesTotal)), "HAL_SAI_Receive_DMA");
    HandleHALResult(HAL_SAI_Transmit_DMA(&hsai_BlockA2, (uint8_t *)AudioInputBuffer, (uint16_t)(AudioData_NumChannels * AudioBuffer_NumSamplesTotal)), "HAL_SAI_Transmit_DMA");
  }
  else
  {
    HandleHALResult(HAL_SAI_Receive_DMA(&hsai_BlockB2, (uint8_t *)AudioInputBuffer, (uint16_t)(AudioData_NumChannels * AudioBuffer_NumSamplesTotal)), "HAL_SAI_Receive_DMA");
    HandleHALResult(HAL_SAI_Transmit_DMA(&hsai_BlockA2, (uint8_t *)AudioOutputBuffer, (uint16_t)(AudioData_NumChannels * AudioBuffer_NumSamplesTotal)), "HAL_SAI_Transmit_DMA");
  }

  DAC_Mute(0);

  while(1)
  {
    if (UseSingleBuffer)
      ToggleLED(LED_Green);

    if (IsUserButtonPressed())
    {
      if (!UserButtonPressed)
      {
        UserButtonPressed = 1;

        switch (ADC_Source)
        {
          case ADC_Source_AR1010:
            ADC_Source = ADC_Source_BlueJack;
            break;
          case ADC_Source_BlueJack:
            ADC_Source = ADC_Source_AR1010;
            break;
          default:
            break;
        }

        ADC_SelectSource(ADC_Source);
      }
    }
    else
      UserButtonPressed = 0;

    if (XPT2046_Sample(&Touch_RawX, &Touch_RawY, &Touch_RawZ))
    {
      XPT2046_ConvertRawToScreen(Touch_RawX, Touch_RawY, &Touch_ScreenX, &Touch_ScreenY);
      AudioTransferGain = Clamp_float((float)Touch_ScreenY / 320.0f, 0.0f, 1.0f);

      PA_MasterVolume_dB = 20.0f - 80.0f * (1.0f - ((float)Touch_ScreenX / 240.0f));
      PA_SetMasterVolume(PA_MasterVolume_dB);
    }

    sprintf(S, "Transfer gain: %0.2f     ", AudioTransferGain);
    ILI9341_DrawTextAtXY(S, 0, 7 * ILI9341_GetFontYSpacing(), tpLeft);

    sprintf(S, "PA master vol: %0.1f     ", PA_MasterVolume_dB);
    ILI9341_DrawTextAtXY(S, 0, 8 * ILI9341_GetFontYSpacing(), tpLeft);

    PA_ErrorStatus = PA_GetErrorStatus();
    sprintf(S, "PA error status: %2X", PA_ErrorStatus);
    ILI9341_DrawTextAtXY(S, 0, 9 * ILI9341_GetFontYSpacing(), tpLeft);

//    HAL_Delay(100);
  }
}
