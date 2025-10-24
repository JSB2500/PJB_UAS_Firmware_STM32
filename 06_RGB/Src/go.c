#include "string.h"
#include "math.h" 
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

#define RGBDataLength 720
uint16_t RGBData[RGBDataLength];
uint16_t RGBDataIndex;

void HandleHALError(HAL_StatusTypeDef Value, char *pText)
{
  if (Value != HAL_OK)
  {
//    LCD_UsrLog("HAL error: %s\n", pText);
//    Error_Handler();
    HAL_GPIO_TogglePin(LED_Red_GPIO_Port, LED_Red_Pin); // !!!
  }
}

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

const int NumLEDs=24;

void SetColor(uint8_t Color_RGB)
{
  uint32_t Color_BGR;

  Color_BGR = 0x000002 << 8 * (Color_RGB - 1);

  // This pattern is useful for ensuring the protocol is correct. Intended for the first 6 LEDs only:
	//  RGB_Begin();
	//  RGB_AppendColour(Color_BGR);
	//  RGB_AppendColour(0x010101);
	//  RGB_AppendColour(Color_BGR);
	//  RGB_AppendColour(Color_BGR);
	//  RGB_AppendColour(0x010101);
	//  RGB_AppendColour(Color_BGR);
	//  RGB_End();

  // This pattern is useful for ensuring all RGBs are working:
  RGB_Begin();
  for (int Index=0;Index<NumLEDs;++Index)
    RGB_AppendColour(Color_BGR);
  RGB_End();

  SetLED(LED_Red, Color_RGB == clRed);
  SetLED(LED_Green, Color_RGB == clGreen);
  SetLED(LED_Blue, Color_RGB == clBlue);
}

void Go()
{
  uint8_t ButtonPressed = 0;
  Color_t Color;

  RGB_SetNumber(NumLEDs);

  SetColor(clRed);
  HAL_Delay(1000);
  SetColor(clGreen);
  HAL_Delay(1000);
  SetColor(clBlue);
  HAL_Delay(1000);

  Color = clRed;
  SetColor(Color);

  while(1)
  {
    if (IsUserButtonPressed())
    {
      if (!ButtonPressed)
      {
//        if (Color == clRed)
//          Color = clBlue;
//        else
//          --Color;

        if (Color == clBlue)
          Color = clRed;
        else
          ++Color;

        SetColor(Color);

        ButtonPressed = 1;
      }
    }
    else
      ButtonPressed = 0;

    HAL_Delay(100);
  }
}
