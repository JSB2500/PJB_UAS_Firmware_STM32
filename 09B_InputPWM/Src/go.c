#include "stdio.h"
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

///////////////////////////////////////////////////////////////////////////////

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim8)
  {
    ToggleLED(LED_Blue);
  }
}

///////////////////////////////////////////////////////////////////////////////

void Go()
{
  char S[200];
  
  ILI9341_SetFont(&FreeSans9pt7b);
  ILI9341_SetTextColor(ILI9341_COLOR_WHITE);
  ILI9341_SetTextBackgroundColor(ILI9341_COLOR_BLACK);
  ILI9341_SetTextDrawMode(tdmAnyCharBar); // Slower but enables flicker free update.

  ILI9341_Clear(0x0000);

  ILI9341_DrawTextAtXY("InputPWM", 0, ILI9341_GetFontYSpacing(), tpLeft);
  ILI9341_DrawTextAtXY("Input to TIM8_CH2 (pin 64)", 0, 2 * ILI9341_GetFontYSpacing(), tpLeft);

  if (HAL_TIM_Base_Start(&htim8))
    Error_Handler();
  if (HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_2))
    Error_Handler();
  if (HAL_TIM_IC_Start(&htim8, TIM_CHANNEL_1))
    Error_Handler();

  while(1)
  {
    sprintf(S, "tim8ch1: %lu             ", HAL_TIM_ReadCapturedValue(&htim8, TIM_CHANNEL_1));
    ILI9341_DrawTextAtXY(S, 0, 4 * ILI9341_GetFontYSpacing(), tpLeft);
    sprintf(S, "tim8ch2: %lu             ", HAL_TIM_ReadCapturedValue(&htim8, TIM_CHANNEL_2));
    ILI9341_DrawTextAtXY(S, 0, 5 * ILI9341_GetFontYSpacing(), tpLeft);

    HAL_Delay(1); //!!! 100
  }
}

