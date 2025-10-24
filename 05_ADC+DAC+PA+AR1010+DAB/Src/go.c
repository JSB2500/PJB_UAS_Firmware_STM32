#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_sai.h"
#include "stm32f7xx_hal_dma.h"
#include "i2c.h"
#include "spi.h"
#include "sai.h"
#include "main.h"
#include "PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.h"
#include "JSB_ILI9341.h"
#include "JSB_XPT2046.h"
#include "Si468x/Si468x.h"
#include "Si468x/Si468x_FM.h"
#include "Si468x/Si468x_DAB.h"
#include "PJB_SST25V.h"
#include "AR1010.h"
#include "gfxfont.h"
#include "FreeSans9pt7b.h"
#include "FreeSans12pt7b.h"
#include "JSB_General.h"
#include "go.h"

static float AR1010_Frequency = 0.0f;

///////////////////////////////////////////////////////////////////////////////

typedef enum
{
  mdNone,
  mdAuxIn,
  mdAR1010,
  mdDAB,
  mdNumModes
} Mode_t;

static Mode_t Mode = mdNone;

/*
 static char *ModeToString(char *S, Mode_t Mode)
{
  switch (Mode)
  {
    case mdAuxIn:
      strcpy(S, "Aux in");
      break;

    case mdDAB:
      strcpy(S, "DAB");
      break;

    case mdAR1010:
      strcpy(S, "AR1010");
      break;

    default:
      S[0] = 0;
      break;
  }

  return S;
}
*/

///////////////////////////////////////////////////////////////////////////////
// RGB

#define RGBDataLength 720
uint16_t RGBData[RGBDataLength];
uint16_t RGBDataIndex;

typedef enum
{
  clNone,
  clRed,
  clGreen,
  clBlue
} Color_t;

char *ColorToString(char *S, Color_t Color)
{
  switch(Color)
  {
    case clRed: strcpy(S, "Red"); break;
    case clGreen: strcpy(S, "Green"); break;
    case clBlue: strcpy(S, "Blue"); break;
    default: S="";
  }

  return S;
}

void SetColor(uint32_t Color)
{
  RGB_Begin();
  RGB_AppendColour(Color);
  RGB_AppendColour(0x010101);
  RGB_AppendColour(Color);
  RGB_AppendColour(Color);
  RGB_AppendColour(0x010101);
  RGB_AppendColour(Color);
  RGB_End();
}

///////////////////////////////////////////////////////////////////////////////

uint8_t ChangeService;
#define DAB_ServiceNameMaxLength 128
char DAB_ServiceName[DAB_ServiceNameMaxLength] = "";

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == SI_INT_Pin)
    si468x_interrupt();
  if (GPIO_Pin == NUserButton_Pin)
    ChangeService = 1;
}

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
  uint16_t Y = 0;
  int16_t Touch_RawX, Touch_RawY, Touch_RawZ;
  int16_t Touch_ScreenX, Touch_ScreenY;
  //!!! DAB_DigRad_Status DAB_Status;
  char S[256];

  RGB_SetNumber(6);
  SetColor(clBlue);
  
  HAL_GPIO_WritePin(DAC_XSMT_GPIO_Port, DAC_XSMT_Pin, 1);
  ILI9341_SetFont(&FreeSans9pt7b);
  ILI9341_SetTextColor(ILI9341_COLOR_WHITE);
  ILI9341_SetTextBackgroundColor(ILI9341_COLOR_BLACK);
  ILI9341_SetTextDrawMode(tdmAnyCharBar); // Slower but enables flicker free update.

  ILI9341_Clear(0x0000);

  uint8_t UserButtonPressed = 0;

  Y += ILI9341_GetFontYSpacing();
  ILI9341_DrawTextAtXY("ADC+DAC+PA+AR1010+DAB", 0, Y, tpLeft);

  Y += ILI9341_GetFontYSpacing();
  if (UseSingleBuffer)
    ILI9341_DrawTextAtXY("Using single buffer", 0, Y, tpLeft);
  else
    ILI9341_DrawTextAtXY("Using I&O buffers", 0, Y, tpLeft);

  ADC_Initialize();
  //
  DAC_Initialize();
  //
  AR1010_init();
  AR1010_set_volume(18);
  //
  si468x_init(Si468x_MODE_DAB);
  Y += ILI9341_GetFontYSpacing();
  ILI9341_DrawTextAtXY("Si4684 booted", 0, Y, tpLeft);

  uint8_t *current_uuid = (uint8_t *) "8f74f7d6-eba6-4a72-bd43-16f123bbc3e6";

  uint8_t read_uuid[36];
  SST25_ReadBlock(0, read_uuid, 36);

  if (IsUserButtonPressed() || strncmp((char *) current_uuid, (char *) read_uuid, 36)) // If flash data either not written or wrong version
  {
    SST25_EraseSector_4K(0);
    SST25_WriteBlock(0, current_uuid, 36);

    si468x_DAB_band_scan();
  }
  else
    HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, 1);
  //
  PA_Initialize(0);
  //PA_Initialize(1); // Headphones.
  PA_SetMasterVolume(PA_MasterVolume_dB);

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

  uint16_t num_services = 0, current_service_id = 0;
  SST25_ReadBlock(4096, (uint8_t *) &num_services, 2);
  ChangeService = 1;

  Mode = mdDAB;
  uint8_t ModeChanged = 1;
  uint16_t Y_Loop  = Y + ILI9341_GetFontYSpacing();
  AR1010_Frequency = 101.7; // JSB: Was 96.4. Sheffield: 88.6 for Radio Sheffield. 92.1 for Radio 3. 101.7 for ClassicFM.
  AR1010_auto_tune(AR1010_Frequency, 1);

  while(1)
  {
    Y = Y_Loop;

    if (Mode == mdAR1010)
    {
//!!!
//      AR1010_Frequency = AR1010_Frequency + 0.1;
//      AR1010_auto_tune(AR1010_Frequency, 1);
//      HAL_Delay(1000);
    }

    if (ModeChanged)
    {
      ILI9341_DrawBar(0, Y_Loop, 240, 430 - Y_Loop, ILI9341_COLOR_BLACK);

      ModeChanged = 0;
      switch(Mode)
      {
        case mdAR1010:
          ADC_SelectSource(ADC_Source_AR1010);
          I2S_SelectSource(I2S_Source_ADC);
          break;

        case mdAuxIn:
          ADC_SelectSource(ADC_Source_BlueJack);
          I2S_SelectSource(I2S_Source_ADC);
          break;

        case mdDAB:
          I2S_SelectSource(I2S_Source_Si4684);
          break;

        default:
          break;
      }
    }

    if (ChangeService)
    {
      // Temp!!!
//      TalkToESP = 1;

      switch (Mode)
      {
        case mdAR1010:
//          AR1010_seek(); //!!!
          break;

        case mdDAB:
          si468x_DAB_tune_service(current_service_id);
          JSB_si468x_load_service_name_from_flash(current_service_id, DAB_ServiceName, DAB_ServiceNameMaxLength);

          ++current_service_id;
          if (current_service_id >= num_services)
            current_service_id = 0;
          break;

        default:
          break;
      }

      ChangeService = 0;
    }

    if (UseSingleBuffer)
      ToggleLED(LED_Green);

    if (IsUserButtonPressed())
    {
      if (!UserButtonPressed)
      {
        UserButtonPressed = 1;
        Mode = Mode + 1;
        if (Mode == mdNumModes)
          Mode =1;
        ModeChanged = 1;
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

//    ModeToString(S1, Mode);
//    sprintf(S, "Mode: %s     ", S1);
//    Y += ILI9341_GetFontYSpacing();
//    ILI9341_DrawTextAtXY(S, 0, Y, tpLeft);
//
//    sprintf(S, "Transfer gain: %0.2f     ", AudioTransferGain);
//    Y += ILI9341_GetFontYSpacing();
//    ILI9341_DrawTextAtXY(S, 0, Y, tpLeft);
//
//    sprintf(S, "PA master vol: %0.1f     ", PA_MasterVolume_dB);
//    Y += ILI9341_GetFontYSpacing();
//    ILI9341_DrawTextAtXY(S, 0, Y, tpLeft);
//
//    if (Mode == mdAR1010)
//    {
//      sprintf(S, "Frequency: %0.1f    ", AR1010_Frequency);
//      Y += ILI9341_GetFontYSpacing();
//      ILI9341_DrawTextAtXY(S, 0, Y, tpLeft);
//    }
//
    if (Mode == mdDAB)
    {
      sprintf(S, "Name: %s         ", DAB_ServiceName);
      Y += ILI9341_GetFontYSpacing();
      ILI9341_DrawTextAtXY(S, 0, Y, tpLeft);

//      si468x_DAB_get_digrad_status(&DAB_Status);
//      //
//      sprintf(S, "DAB RSSI: %d    ", (int8_t)DAB_Status.data[2]);
//      Y += ILI9341_GetFontYSpacing();
//      ILI9341_DrawTextAtXY(S, 0, Y, tpLeft);
//      //
//      sprintf(S, "DAB SNR: %d    ", (int8_t)DAB_Status.data[3]);
//      Y += ILI9341_GetFontYSpacing();
//      ILI9341_DrawTextAtXY(S, 0, Y, tpLeft);
//      //
//      sprintf(S, "DAB FIC_QUALITY: %d    ", (int8_t)DAB_Status.data[4]);
//      Y += ILI9341_GetFontYSpacing();
//      ILI9341_DrawTextAtXY(S, 0, Y, tpLeft);
//      //
//      sprintf(S, "DAB CNR: %d    ", (int8_t)DAB_Status.data[5]);
//      Y += ILI9341_GetFontYSpacing();
//      ILI9341_DrawTextAtXY(S, 0, Y, tpLeft);
    }

    uint32_t PA_LeftMagnitude, PA_RightMagnitude;
    PA_ReadPWMLevelMeter(&PA_LeftMagnitude, &PA_RightMagnitude);
    float Brightness1 = Clamp_float((float)PA_LeftMagnitude / 10000000.0f, 0.0f, 1.0f);
    float Brightness2 = Brightness1 * Brightness1;
    float Brightness3 = Brightness1 * Brightness2;
    uint32_t Color = ((uint8_t) round(255 * Brightness2)) << 16 | ((uint8_t) round(255 * Brightness3)) << 8 | ((uint8_t)round(255 * Brightness1));
    SetColor(Color);

//    HAL_Delay(100);
  }
}
