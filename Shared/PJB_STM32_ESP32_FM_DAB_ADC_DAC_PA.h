#ifndef PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA_H_
#define PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA_H_

///////////////////////////////////////////////////////////////////////////////

extern void (*pGenSPI_Begin)();
extern void (*pGenSPI_End)();

///////////////////////////////////////////////////////////////////////////////

void HandleHALResult(HAL_StatusTypeDef Value, char *pText);

///////////////////////////////////////////////////////////////////////////////
// MCU stuff:

uint32_t GetAPB1TimerClockFrequency();

///////////////////////////////////////////////////////////////////////////////
// LCD:

void LCD_BacklightOn(uint8_t Value);

///////////////////////////////////////////////////////////////////////////////
// LEDs:

typedef enum
{
  LED_None,
  LED_Red,
  LED_Green,
  LED_Blue
} LED_t;

void SetLED(LED_t LED, uint8_t Value);
void ToggleLED(LED_t LED);

///////////////////////////////////////////////////////////////////////////////
// User button:

uint8_t IsUserButtonPressed();

///////////////////////////////////////////////////////////////////////////////
// I2S_Source:

typedef enum
{
  // Don't change these, e.g. don't add a None, because they are the values output to the hardware.
  I2S_Source_ESP32_A = 0,
  I2S_Source_ESP32_B = 1,
  I2S_Source_Si4684 = 2,
  I2S_Source_ADC = 3
} I2S_Source_t;

void I2S_SelectSource(I2S_Source_t Source);

///////////////////////////////////////////////////////////////////////////////
// ADC:

typedef enum
{
  ADC_Source_None,
  ADC_Source_AR1010,
  ADC_Source_BlueJack
} ADC_Source_t;

uint8_t ADC_RegisterRead(uint8_t Index);
void ADC_RegisterWrite(uint8_t Index, uint8_t Value);
void ADC_SetRegisterPage(uint8_t PageIndex);
void ADC_SetGPIO(uint8_t Value);
void ADC_CheckPowerSupplies();
void ADC_Initialize();
void ADC_SelectSource(ADC_Source_t Source);
void ADC_SetGain(float Value);

///////////////////////////////////////////////////////////////////////////////
// DAC:

void DAC_Initialize();
void DAC_Mute(uint8_t Value);

///////////////////////////////////////////////////////////////////////////////
// PA:

void PA_Initialize(uint8_t HeadphoneMode);
uint8_t PA_GetErrorStatus();
void PA_ClearErrorStatus();
void PA_SetMasterVolume(float Volume_dB);
void PA_Mute();
void PA_ReadPWMLevelMeter(uint32_t *pLeft, uint32_t *pRight);
float PA_Convert323FormatToFloat(uint32_t Value);

///////////////////////////////////////////////////////////////////////////////
// ESP32s:

void ESP32_A_Enable(uint8_t Value);
void ESP32_B_Enable(uint8_t Value);

///////////////////////////////////////////////////////////////////////////////
// RGBs:

void RGB_SetNumber(uint8_t Value);
void RGB_Begin();
void RGB_AppendColour(uint32_t Color);
void RGB_End();

///////////////////////////////////////////////////////////////////////////////
// Test points:

void TestPoint_Set(uint8_t Value);

///////////////////////////////////////////////////////////////////////////////

#endif
