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
#include "JSB_SPDIFRX.h"
#include "AR1010.h"
#include "gfxfont.h"
#include "FreeSans9pt7b.h"
#include "FreeSans12pt7b.h"
#include "JSB_General.h"
#include "go.h"

///////////////////////////////////////////////////////////////////////////////

float AudioTransferGain = 1.0f;
float PA_MasterVolume_dB = 0.0f;

AudioSample24_t AudioOutputBuffer[SPDIFRX_Buffer_NumSamples];

uint32_t DACSampleRate = 0;

///////////////////////////////////////////////////////////////////////////////
// Overrides:

void SPDIFRX_Locked_Callback(uint8_t Value)
{
  HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, Value);
}

void Mute(uint8_t Value);

void SPDIFRX_MuteOutput_Callback(uint8_t Value)
{
  Mute(Value);
}

void SetDACSampleRate_44100();
void SetDACSampleRate_48000();

void SPDIFRX_SetOutputSampleRate_Callback(uint32_t Value)
{
  if (Value == 44100)
    SetDACSampleRate_44100();
  else if (Value == 48000)
    SetDACSampleRate_48000();
  else
    DACSampleRate = 0;
}

void ClearOutputBuffer();

void SPDIFRX_ClearOutputBuffer()
{
  ClearOutputBuffer();
}

///////////////////////////////////////////////////////////////////////////////

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  SPDIFRX_CopySamplesToGivenBuffer(AudioOutputBuffer, 0);
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
  SPDIFRX_CopySamplesToGivenBuffer(AudioOutputBuffer, 1);
}

///////////////////////////////////////////////////////////////////////////////

void ClearOutputBuffer()
{
  AudioSample24_t ZeroSample={0,0};
  for(uint32_t SampleIndex = 0; SampleIndex < SPDIFRX_Buffer_NumSamples; ++SampleIndex)
    AudioOutputBuffer[SampleIndex] = ZeroSample;
}

void Mute(uint8_t Value)
{
  DAC_Mute(Value);

  if (Value)
    PA_Mute();
  else
    PA_SetMasterVolume(PA_MasterVolume_dB);
}

void SetDACSampleRate_44100()
{
  if (DACSampleRate == SAI_AUDIO_FREQUENCY_44K)
    return;

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  HAL_RCCEx_GetPeriphCLKConfig(&PeriphClkInitStruct);

  PeriphClkInitStruct.PLLI2S.PLLI2SN = 429;
  PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLI2SP_DIV4;
  PeriphClkInitStruct.PLLI2S.PLLI2SQ = 2;
  PeriphClkInitStruct.PLLI2SDivQ = 19;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    Error_Handler();

  hsai_BlockA2.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_44K;
  DACSampleRate = 44100;

  HandleHALResult(HAL_SAI_DMAStop(&hsai_BlockA2), "HAL_SAI_DMAStop");
  HandleHALResult(HAL_SAI_InitProtocol(&hsai_BlockA2, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_24BIT, 2), "HAL_SAI_InitProtocol");
  HandleHALResult(HAL_SAI_Transmit_DMA(&hsai_BlockA2, (uint8_t *)AudioOutputBuffer, (sizeof(AudioSample24_t) / sizeof(int32_t)) * SPDIFRX_Buffer_NumSamples), "HAL_SAI_Transmit_DMA");
}

void SetDACSampleRate_48000()
{
  if (DACSampleRate == SAI_AUDIO_FREQUENCY_48K)
    return;

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  HAL_RCCEx_GetPeriphCLKConfig(&PeriphClkInitStruct);

  PeriphClkInitStruct.PLLI2S.PLLI2SN = 344;
  PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLI2SP_DIV4;
  PeriphClkInitStruct.PLLI2S.PLLI2SQ = 14;
  PeriphClkInitStruct.PLLI2SDivQ = 1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    Error_Handler();

  hsai_BlockA2.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_48K;
  DACSampleRate = SAI_AUDIO_FREQUENCY_48K;

  HandleHALResult(HAL_SAI_DMAStop(&hsai_BlockA2), "HAL_SAI_DMAStop");
  HandleHALResult(HAL_SAI_InitProtocol(&hsai_BlockA2, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_24BIT, 2), "HAL_SAI_InitProtocol");
  HandleHALResult(HAL_SAI_Transmit_DMA(&hsai_BlockA2, (uint8_t *)AudioOutputBuffer, (sizeof(AudioSample24_t) / sizeof(int32_t)) * SPDIFRX_Buffer_NumSamples), "HAL_SAI_Transmit_DMA");
}

void Go()
{
  uint16_t Y = 0;
  int16_t Touch_RawX, Touch_RawY, Touch_RawZ;
  int16_t Touch_ScreenX, Touch_ScreenY;
  uint8_t PA_ErrorStatus;
  uint8_t SPDIFRX_Started = 0;
  uint8_t SPDIF_InputChannel = 2;
  uint8_t UserButtonPressedAlreadyPressed = 0;
  uint8_t RestartRequired = 0;
  uint8_t NumTransitionErrors, NumSyncErrors, NumFrameErrors;
  char S[128];
  
  ILI9341_SetFont(&FreeSans12pt7b);
  ILI9341_SetTextColor(ILI9341_COLOR_WHITE);
  ILI9341_SetTextBackgroundColor(ILI9341_COLOR_BLACK);
  ILI9341_SetTextDrawMode(tdmAnyCharBar); // Slower but enables flicker free update.
  ILI9341_SetFont(&FreeSans9pt7b);

  ILI9341_Clear(0x0000);

  DAC_Initialize();
  //
  PA_Initialize(0);
  //PA_Initialize(1); // Headphones.

  Y += ILI9341_GetFontYSpacing();
  ILI9341_DrawTextAtXY("S/P-DIF=>DAC+PA", 0, Y, tpLeft);

  Mute(0);

  while(1)
  {
    if (IsUserButtonPressed())
    {
      if (!UserButtonPressedAlreadyPressed)
      {
        SPDIF_InputChannel = SPDIF_InputChannel == 2 ? 3 : 2;
        RestartRequired = 1;
        UserButtonPressedAlreadyPressed = 1;
      }
    }
    else
      UserButtonPressedAlreadyPressed = 0;

    if (!SPDIFRX_Started)
    {
      SPDIFRX_Started = SPDIFRX_Start(hsai_BlockA2.Init.AudioFrequency, SPDIF_InputChannel, 0, 0);
      PA_ClearErrorStatus();
    }

    if (SPDIFRX_Started)
    {
      if (SPDIFRX_HasReceiveErrorOccurred() || RestartRequired)
      {
        RestartRequired = 0;
        SPDIFRX_Stop();
        DACSampleRate = 0;
        SPDIFRX_Started = 0;
      }
    }

    if (XPT2046_Sample(&Touch_RawX, &Touch_RawY, &Touch_RawZ))
    {
      XPT2046_ConvertRawToScreen(Touch_RawX, Touch_RawY, &Touch_ScreenX, &Touch_ScreenY);
      AudioTransferGain = Clamp_float((float)Touch_ScreenY / 320.0f, 0.0f, 1.0f);
      SPDIFRX_TransferGain = AudioTransferGain;

      PA_MasterVolume_dB = 20.0f - 80.0f * (1.0f - ((float)Touch_ScreenX / 240.0f));
      if (SPDIFRX_Started) // Don't override mute. Would be better if PA had a mute function that is independent of the set volume.
        PA_SetMasterVolume(PA_MasterVolume_dB);
    }

    int LineIndex = 3;

    sprintf(S, "SPDIF input channel: %d     ", SPDIF_InputChannel);
    ILI9341_DrawTextAtXY(S, 0, LineIndex++ * ILI9341_GetFontYSpacing(), tpLeft);

    sprintf(S, "Transfer gain: %0.2f     ", AudioTransferGain);
    ILI9341_DrawTextAtXY(S, 0, LineIndex++ * ILI9341_GetFontYSpacing(), tpLeft);

    sprintf(S, "PA master vol: %0.1f     ", PA_MasterVolume_dB);
    ILI9341_DrawTextAtXY(S, 0, LineIndex++ * ILI9341_GetFontYSpacing(), tpLeft);

    PA_ErrorStatus = PA_GetErrorStatus();
    sprintf(S, "PA error status: %2X", PA_ErrorStatus);
    ILI9341_DrawTextAtXY(S, 0, LineIndex++ * ILI9341_GetFontYSpacing(), tpLeft);

    sprintf(S, "DAC sample rate: %lu          ", DACSampleRate);
    ILI9341_DrawTextAtXY(S, 0, LineIndex++ * ILI9341_GetFontYSpacing(), tpLeft);

    SPDIFRX_GetNumErrors(&NumTransitionErrors, &NumSyncErrors, &NumFrameErrors);

    sprintf(S, "Num transition errors: %d          ", NumTransitionErrors);
    ILI9341_DrawTextAtXY(S, 0, LineIndex++ * ILI9341_GetFontYSpacing(), tpLeft);

    sprintf(S, "Num sync errors: %d          ", NumSyncErrors);
    ILI9341_DrawTextAtXY(S, 0, LineIndex++ * ILI9341_GetFontYSpacing(), tpLeft);

    sprintf(S, "Num frame errors: %d          ", NumFrameErrors);
    ILI9341_DrawTextAtXY(S, 0, LineIndex++ * ILI9341_GetFontYSpacing(), tpLeft);
  }
}
