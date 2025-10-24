#include "string.h"
#include "stdlib.h"
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
#include "spiritMP3Dec.h"
#include "go.h"

typedef struct
{
  int32_t Left, Right;
} AudioData_Sample_t;

#define AudioDMA_CircularBuffer_NumSamplesPerHalf 32
#define AudioDMA_CircularBuffer_NumSamplesInTotal (AudioDMA_CircularBuffer_NumSamplesPerHalf * 2) /* Derived */
static AudioData_Sample_t AudioDMA_CircularBuffer_Samples[AudioDMA_CircularBuffer_NumSamplesInTotal];

static uint32_t MP3Source_NumBytes = 0;
static void *MP3Source_pStart = NULL;
static void *MP3Source_pCurrent = NULL;
static void *MP3Source_pFirstGoodStreamFrame = NULL;

static TSpiritMP3Decoder *MP3Decoder_pInstance = NULL;
static TSpiritMP3Info *MP3Decoder_pMP3Info = NULL;

static unsigned int MP3Decoder_GetDataCallback(void *pData,unsigned int NumBytesWanted, void *pToken)
{
  uint32_t NumBytesToGet;
  uint32_t NumBytesSupplied = sizeof (char);

  uint32_t NumBytesRemaining = (MP3Source_NumBytes - (MP3Source_pCurrent - MP3Source_pStart));

  NumBytesToGet = NumBytesWanted;
  if (NumBytesToGet > NumBytesRemaining)
    NumBytesToGet = NumBytesRemaining;

  memcpy(pData, MP3Source_pCurrent, NumBytesToGet);
  NumBytesSupplied = NumBytesToGet;

  MP3Source_pCurrent += NumBytesSupplied;

  return NumBytesSupplied;
}

static void AudioDMA_FillBufferWithSamplesFromMP3Decoder(uint32_t DestinationBufferOffset, uint32_t NumSamples)
{
  short MP3Decoder_pOutputSamples[2 * NumSamples * sizeof(short)];

  //!!! Warning! Doesn't check for success!
  uint32_t NumSamplesDecoded = SpiritMP3Decode(MP3Decoder_pInstance, (short *)MP3Decoder_pOutputSamples, NumSamples, NULL);

  short *pMP3OutputSamples = MP3Decoder_pOutputSamples;
  AudioData_Sample_t *pDestination = &AudioDMA_CircularBuffer_Samples[DestinationBufferOffset];
  for (uint32_t SampleIndex = 0; SampleIndex < NumSamplesDecoded; ++SampleIndex)
  {
    short Left16 = *pMP3OutputSamples++;
    short Right16 = *pMP3OutputSamples++;
    pDestination->Left = Left16 << 16;
    pDestination->Right = Right16 << 16;
    ++pDestination;
  }
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai == &hsai_BlockA2)
  {
    AudioDMA_FillBufferWithSamplesFromMP3Decoder(0, AudioDMA_CircularBuffer_NumSamplesPerHalf);
  }
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
  if (hsai == &hsai_BlockA2)
  {
    AudioDMA_FillBufferWithSamplesFromMP3Decoder(AudioDMA_CircularBuffer_NumSamplesPerHalf, AudioDMA_CircularBuffer_NumSamplesPerHalf);
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
  ILI9341_DrawTextAtXY("MP3=>DAC", 0, Y, tpLeft);

  DAC_Initialize();

  // Get MP3 source info:
  MP3Source_NumBytes = *(uint32_t *)0x8100000;
  MP3Source_pStart = (uint32_t *)0x8100004;
  MP3Source_pCurrent = MP3Source_pStart;

  // Initialize MP3 decoder:
  MP3Decoder_pInstance = (TSpiritMP3Decoder*)malloc(sizeof(TSpiritMP3Decoder)); /* 0x3038 bytes */
  if (!MP3Decoder_pInstance)
    Error_Handler();
  MP3Decoder_pMP3Info = (TSpiritMP3Info*)malloc(sizeof(TSpiritMP3Info));
  if (!MP3Decoder_pMP3Info)
    Error_Handler();
  short *MP3Decoder_pOutputSamples = (short*)malloc(2 * 1152 * sizeof(short));
  if (!MP3Decoder_pOutputSamples)
    Error_Handler();
  //
  SpiritMP3DecoderInit(MP3Decoder_pInstance, &MP3Decoder_GetDataCallback, NULL, NULL);
  do
  {
    MP3Source_pFirstGoodStreamFrame = MP3Source_pCurrent;
    SpiritMP3Decode(MP3Decoder_pInstance, (short *)MP3Decoder_pOutputSamples, 1152, MP3Decoder_pMP3Info);
  } while (!((MP3Decoder_pMP3Info->IsGoodStream) && (MP3Decoder_pMP3Info->nBitrateKbps) && (MP3Decoder_pMP3Info->nSampleRateHz)));

  MP3Source_pCurrent = MP3Source_pFirstGoodStreamFrame;

  DAC_Mute(0);

  HAL_SAI_TxHalfCpltCallback(&hsai_BlockA2); // Prime DMA buffer.

  SCB_CleanDCache_by_Addr((uint32_t *)AudioDMA_CircularBuffer_Samples, AudioDMA_CircularBuffer_NumSamplesInTotal * sizeof(AudioData_Sample_t));
  HAL_StatusTypeDef HAL_Result = HAL_SAI_Transmit_DMA(&hsai_BlockA2, (uint8_t *)&AudioDMA_CircularBuffer_Samples, (uint16_t)((sizeof(AudioData_Sample_t) / sizeof(int32_t)) * AudioDMA_CircularBuffer_NumSamplesInTotal)); // Each DMA item is 32 bits (set in CubeMX).
  if (HAL_Result != HAL_OK)
    Error_Handler();

  while(1)
  {
    // Indicate playing.
    ToggleLED(LED_Green);
    HAL_Delay(500);
  }
}
