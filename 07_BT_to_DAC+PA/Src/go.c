#include "stdio.h"
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
// Program configuration:

#define UseSingleBuffer 0

///////////////////////////////////////////////////////////////////////////////

float AudioTransferGain = 1.0f;
float PA_MasterVolume_dB = -20.0f;

typedef struct
{
  int16_t Left, Right;
} AudioData_InputSample_t;

typedef struct
{
  int16_t Left, Right;
} AudioData_OutputSample_t;

#define AudioData_NumChannels 2

#define AudioBuffer_NumSamplesPerHalf 16
#define AudioBuffer_NumSamplesTotal (2 * AudioBuffer_NumSamplesPerHalf)

AudioData_InputSample_t AudioInputBuffer[AudioBuffer_NumSamplesTotal];
AudioData_OutputSample_t AudioOutputBuffer[AudioBuffer_NumSamplesTotal];

///////////////////////////////////////////////////////////////////////////////

float I2S_16bitToNormalizedFloat(int16_t Value)
// To +/- 1.0.
{
  return Clamp_float(Value / 32767.0f, -1.0f, 1.0f);
}

float I2S_24bitToNormalizedFloat(int32_t Value)
// To +/- 1.0.
{
  if (Value & 0x00800000)
    Value |= 0xFF000000;

  return Clamp_float(Value / 8388607.0f, -1.0f, 1.0f);
}

int32_t I2S_NormalizedFloatTo16bit(float Value)
// From +/- 1.0.
{
  return round(32767.0f * Clamp_float(Value, -1.0f, 1.0f));
}

int32_t I2S_NormalizedFloatTo24bit(float Value)
// From +/- 1.0.
{
  return round(8388607.0f * Clamp_float(Value, -1.0f, 1.0f));
}

void AudioBuffer_TransferFromInputBufferToOutputBuffer(AudioBufferHalf_t AudioBufferHalf)
{
  uint16_t FirstSampleIndex, SampleIndex;
  float Audio_Left, Audio_Right;
  AudioData_InputSample_t *pInputSample;
  AudioData_OutputSample_t *pOutputSample;

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

    Audio_Left = I2S_16bitToNormalizedFloat(pInputSample->Left);
    Audio_Right = I2S_16bitToNormalizedFloat(pInputSample->Right);

    Audio_Left *= AudioTransferGain;
    Audio_Right *= AudioTransferGain;

    pOutputSample->Left = I2S_NormalizedFloatTo16bit(Audio_Left);
    pOutputSample->Right = I2S_NormalizedFloatTo16bit(Audio_Right);
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
  uint16_t Y = 0;
  int16_t Touch_RawX, Touch_RawY, Touch_RawZ;
  int16_t Touch_ScreenX, Touch_ScreenY;
  I2S_Source_t I2S_Source;
  uint8_t PA_ErrorStatus;
  char S[128];
  
  ILI9341_SetFont(&FreeSans12pt7b);
  ILI9341_SetTextColor(ILI9341_COLOR_WHITE);
  ILI9341_SetTextBackgroundColor(ILI9341_COLOR_BLACK);
  ILI9341_SetTextDrawMode(tdmAnyCharBar); // Slower but enables flicker free update.

  ILI9341_Clear(0x0000);

  ESP32_B_Enable(1);
  //
  I2S_Source = I2S_Source_ESP32_B;
  I2S_SelectSource(I2S_Source);
  //
  ADC_Initialize();
  ADC_SelectSource(ADC_Source_BlueJack);
  ADC_SetGain(12);
  //
  DAC_Initialize();
  //
  PA_Initialize(0);
  //PA_Initialize(1); // Headphones.
  PA_SetMasterVolume(PA_MasterVolume_dB);

  Y += ILI9341_GetFontYSpacing();
  ILI9341_DrawTextAtXY("BT=>DAC+PA", 0, Y, tpLeft);

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

    if (IsUserButtonPressed())
    {
      if (I2S_Source == I2S_Source_ESP32_B)
        I2S_Source = I2S_Source_ADC;
      else
        I2S_Source = I2S_Source_ESP32_B;
      I2S_SelectSource(I2S_Source);
      while (IsUserButtonPressed()) {};
    }

//    HAL_Delay(100);
  }
}
