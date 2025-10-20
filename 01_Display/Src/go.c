#include "string.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_sai.h"
#include "stm32f7xx_hal_dma.h"
#include "i2c.h"
#include "sai.h"
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
  ILI9341_DrawTextAtXY("Testing Testing 1 2 3!", 0, Y, tpLeft);
  Y += ILI9341_GetFontYSpacing();
  ILI9341_DrawTextAtXY("Testing Testing 1 2 3!", 0, Y, tpLeft);
  Y += ILI9341_GetFontYSpacing();
  ILI9341_DrawTextAtXY("Testing Testing 1 2 3!", 0, Y, tpLeft);

  while(1)
  {
    ToggleLED(LED_Green);

    HAL_Delay(500);
  }
}
