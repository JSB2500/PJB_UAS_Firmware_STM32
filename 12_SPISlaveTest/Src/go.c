#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_sai.h"
#include "stm32f7xx_hal_dma.h"
#include "i2c.h"
#include "spi.h"
#include "sai.h"
#include "main.h"
#include "PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.h"
#include "JSB_General.h"
#include "JSB_UI_Elements.h"
#include "go.h"

///////////////////////////////////////////////////////////////////////////////

void Go()
{
  JSB_UI_InitializeDisplay();

  while (1)
  {
    uint8_t SPIData[256];
    char S[256];

    HAL_SPI_Receive(&hspi4, SPIData, 128, HAL_MAX_DELAY);

    ILI9341_Clear(0);

    sprintf(S, "Received SPI data:");
    ILI9341_DrawTextAtXY(S, 0, 20, tpLeft);

    for (int LineIndex = 0; LineIndex < 8; ++LineIndex)
    {
      int Offset = 8 * LineIndex;
      sprintf(S, "%02X %02X %02X %02X", SPIData[Offset + 0], SPIData[Offset + 1], SPIData[Offset + 2], SPIData[Offset + 3]);
      ILI9341_DrawTextAtXY(S, 0, 40 + 20 * LineIndex, tpLeft);
    }

    if (ILI9341_UsingBackBuffer())
      ILI9341_CopyBackBufferToDisplay();
  }
}
