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
#include "JSB_IR_RC5.h"
#include "go.h"

///////////////////////////////////////////////////////////////////////////////

  void Go()
{
  uint8_t IR_Address, IR_Command, IR_Toggle, IR_Error;

  JSB_UI_InitializeDisplay();

  JSB_IR_RC5_Initialize(&htim3);

  while (1)
  {
    char S[256];

    ILI9341_Clear(0);

    sprintf(S, "Receive IR:");
    ILI9341_DrawTextAtXY(S, 0, 1 * ILI9341_GetFontYSpacing(), tpLeft);
    sprintf(S, "Input to Tim3-Ch1 (pin 63):");
    ILI9341_DrawTextAtXY(S, 0, 2 * ILI9341_GetFontYSpacing(), tpLeft);

    if (JSB_IR_RC5_GetIfAvailable(&IR_Address, &IR_Command, &IR_Toggle, &IR_Error))
    {
      sprintf(S, "A: %d C: %d T: %d", IR_Address, IR_Command, IR_Toggle);
      ILI9341_DrawTextAtXY(S, 0, 3 * ILI9341_GetFontYSpacing(), tpLeft);

      if (IR_Error)
      {
        sprintf(S, "Error");
        ILI9341_DrawTextAtXY(S, 0, 4 * ILI9341_GetFontYSpacing(), tpLeft);
      }

      if (ILI9341_UsingBackBuffer())
        ILI9341_CopyBackBufferToDisplay();

      HAL_Delay(100);
    }
  }
}
