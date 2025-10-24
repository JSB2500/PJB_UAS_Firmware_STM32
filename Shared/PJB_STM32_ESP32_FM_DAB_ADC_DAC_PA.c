#include <stdlib.h>
#include <math.h>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_sai.h"
#include "stm32f7xx_hal_dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "main.h"
#include "PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.h"

///////////////////////////////////////////////////////////////////////////////

void (*pGenSPI_Begin)() = NULL;
void (*pGenSPI_End)() = NULL;

///////////////////////////////////////////////////////////////////////////////

void HandleHALResult(HAL_StatusTypeDef Value, char *pText)
{
  if (Value != HAL_OK)
  {
//    LCD_UsrLog("HAL error: %s\n", pText);
//    Error_Handler();
    HAL_GPIO_TogglePin(LED_Red_GPIO_Port, LED_Red_Pin); // !!!
  }
}

///////////////////////////////////////////////////////////////////////////////
// For AR10101.c and Si4684 etc.c:

void I2C_write(uint8_t Address, uint8_t *pData, uint16_t NumBytes)
{
  HandleHALResult(HAL_I2C_Master_Transmit(&hi2c1, Address << 1, pData, NumBytes, 1000), "HAL_I2C_Master_Transmit");
}

void I2C_read(uint8_t Address, uint8_t *pData, uint16_t NumBytes)
{
  HandleHALResult(HAL_I2C_Master_Receive(&hi2c1, Address << 1, pData, NumBytes, 1000), "HAL_I2C_Master_Receive");
}

///////////////////////////////////////////////////////////////////////////////
// MCU stuff:

uint32_t GetAPB1TimerClockFrequency()
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
  uint32_t Result;

  Result = 2 * HAL_RCC_GetPCLK1Freq();

  HAL_RCCEx_GetPeriphCLKConfig(&PeriphClkInit);

  if (PeriphClkInit.TIMPresSelection == RCC_TIMPRES_ACTIVATED)
    Result *= 2;

  return Result;
}

///////////////////////////////////////////////////////////////////////////////
// For SST25V_flash.c:

void SST25_SPI_WriteBlock(uint8_t *pBlock, uint16_t NumBytes)
{
  HAL_SPI_Transmit(&hspi3, pBlock, NumBytes, 1000);
}

void SST25_SPI_ReadBlock(uint8_t *pBlock, uint16_t NumBytes)
{
  HAL_SPI_Receive(&hspi3, pBlock, NumBytes, 1000);
}

uint8_t SST25_SPI_ReadWriteByte(uint8_t Value)
{
  uint8_t Received;

  HAL_SPI_TransmitReceive(&hspi3, &Value, &Received, 1, 1000);

  return Received;
}

void SST25_SPI_Begin()
{
  if (pGenSPI_Begin)
    (*pGenSPI_Begin)();
  HAL_GPIO_WritePin(FLASH_NSS_GPIO_Port, FLASH_NSS_Pin, 0);
}

void SST25_SPI_End()
{
  HAL_GPIO_WritePin(FLASH_NSS_GPIO_Port, FLASH_NSS_Pin, 1);
  if (pGenSPI_End)
    (*pGenSPI_End)();
}

///////////////////////////////////////////////////////////////////////////////
// LCD:

void LCD_BacklightOn(uint8_t Value)
{
  HAL_GPIO_WritePin(LCD_NBACKLIGHT_GPIO_Port, LCD_NBACKLIGHT_Pin, Value ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

///////////////////////////////////////////////////////////////////////////////
// LEDs:

void SetLED(LED_t LED, uint8_t Value)
{
  switch(LED)
  {
    case LED_Red:
      HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, Value);
      break;
    case LED_Green:
      HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, Value);
      break;
    case LED_Blue:
      HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, Value);
      break;
    default:
      Error_Handler();
      break;
  }
}

void ToggleLED(LED_t LED)
{
  switch(LED)
  {
    case LED_Red:
      HAL_GPIO_TogglePin(LED_Red_GPIO_Port, LED_Red_Pin);
      break;
    case LED_Green:
      HAL_GPIO_TogglePin(LED_Green_GPIO_Port, LED_Green_Pin);
      break;
    case LED_Blue:
      HAL_GPIO_TogglePin(LED_Blue_GPIO_Port, LED_Blue_Pin);
      break;
    default:
      Error_Handler();
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////

void HandleError()
{
  ToggleLED(LED_Red);
}

///////////////////////////////////////////////////////////////////////////////
// User button:

uint8_t IsUserButtonPressed()
{
  return !HAL_GPIO_ReadPin(NUserButton_GPIO_Port, NUserButton_Pin);
}

///////////////////////////////////////////////////////////////////////////////
// I2S_Source:

void I2S_SelectSource(I2S_Source_t Source)
{
  HAL_GPIO_WritePin(MUX_S0_GPIO_Port, MUX_S0_Pin, Source & 0x01);
  HAL_GPIO_WritePin(MUX_S1_GPIO_Port, MUX_S1_Pin, Source & 0x02);
}

///////////////////////////////////////////////////////////////////////////////
// ADC (PCM1865):

#define ADC_I2C_Address 0x4A

uint8_t ADC_RegisterRead(uint8_t Index)
{
  uint8_t Result;
  HAL_StatusTypeDef HAL_Result;

  HAL_Result = HAL_I2C_Mem_Read(&hi2c1, ADC_I2C_Address << 1, Index, 1, &Result, 1, 1000);
  if (HAL_Result != HAL_OK)
    HandleError();

  return Result;
}

void ADC_RegisterWrite(uint8_t Index, uint8_t Value)
{
  uint8_t Data[2];
  HAL_StatusTypeDef HAL_Result;

  Data[0] = Index;
  Data[1] = Value;

  HAL_Result = HAL_I2C_Master_Transmit(&hi2c1, ADC_I2C_Address << 1, Data, 2, 1000);
  if (HAL_Result != HAL_OK)
    HandleError();
}

void ADC_SetRegisterPage(uint8_t PageIndex)
{
  ADC_RegisterWrite(0x00, PageIndex);
}

void ADC_SetGPIO(uint8_t Value)
{
  ADC_RegisterWrite(0x14, (Value & 0x0F) << 4);
}

void ADC_CheckPowerSupplies()
{
  uint8_t Value;

  Value = ADC_RegisterRead(0x78);

  if (!(Value || ~0x07))
    HandleError();
}

void ADC_Initialize()
{
//  // !!! Investigate! Reset not working!
//  ADC_RegisterWrite(0x06, 0x45);
//  ADC_RegisterWrite(0x00, 0xFF); // Reset registers.
//  ADC_RegisterWrite(0x00, 0x00); // Reset registers.
//  volatile uint8_t A = ADC_RegisterRead(0x06);

// !!! Not working!
//  ADC_RegisterWrite(0x00, 0xFF); // Reset registers.

  ADC_RegisterWrite(0x00, 0x00); // Default to bank 0.

  ADC_CheckPowerSupplies();

  ADC_SelectSource(ADC_Source_None);
  ADC_SetGain(0);
  ADC_RegisterWrite(0x10, 0x00); // Set GPIO 1 and 0 to GPIO.
  ADC_RegisterWrite(0x11, 0x00); // Set GPIO 3 and 2 to GPIO.
  ADC_RegisterWrite(0x12, 0x44); // Set GPIO 1 and 0 to be outputs.
  ADC_RegisterWrite(0x13, 0x44); // Set GPIO 3 and 2 to be outputs.
}

void ADC_SelectSource(ADC_Source_t Source)
{
  uint8_t WriteValue;

  switch(Source)
  {
    case ADC_Source_AR1010:
      WriteValue = 0x41;
      break;

    case ADC_Source_BlueJack:
      WriteValue = 0x42;
      break;

    default:
      WriteValue = 0x40;
      break;
  }

  ADC_RegisterWrite(0x06, WriteValue);
  ADC_RegisterWrite(0x07, WriteValue);
}

void ADC_SetGain(float Value)
// -12dB to +32dB in 0.5dB steps.
{
  uint8_t RegisterValue;

  if ((Value < -12.0f) || (Value > 32.0f))
    Error_Handler();

  RegisterValue = (int)round(2.0f * Value);

  ADC_RegisterWrite(0x01, RegisterValue);
  ADC_RegisterWrite(0x02, RegisterValue);
}

///////////////////////////////////////////////////////////////////////////////
// DAC (PCM1502A):

void DAC_Initialize()
{
  HAL_GPIO_WritePin(DAC_FLT_GPIO_Port, DAC_FLT_Pin, 0); // Latency.
  HAL_GPIO_WritePin(DAC_DEMP_GPIO_Port, DAC_DEMP_Pin, 0); // De-emphasis for 44.1kHz.
  DAC_Mute(1);
}

void DAC_Mute(uint8_t Value)
{
  HAL_GPIO_WritePin(DAC_XSMT_GPIO_Port, DAC_XSMT_Pin, Value ? 0 : 1);
}

///////////////////////////////////////////////////////////////////////////////
// PA (TAS5719):

#define PA_I2C_Address 0x2A

void PA_RegisterWrite8(uint8_t Index, uint8_t Value)
{
  HandleHALResult(HAL_I2C_Mem_Write(&hi2c1, PA_I2C_Address << 1, Index, 1, &Value, 1, 1000), "HAL_I2C_Mem_Write");
}

uint8_t PA_RegisterRead8(uint8_t Index)
{
  uint8_t Result;

  HandleHALResult(HAL_I2C_Mem_Read(&hi2c1, PA_I2C_Address << 1, Index, 1, &Result, 1, 1000), "HAL_I2C_Mem_Read");

  return Result;
}

void PA_RegisterWrite16(uint8_t Index, uint16_t Value)
{
  uint8_t Data[2];

  Data[0] = (uint8_t)(Value >> 8);
  Data[1] = (uint8_t)(Value);

  HandleHALResult(HAL_I2C_Mem_Write(&hi2c1, PA_I2C_Address << 1, Index, 1, Data, 2, 1000), "HAL_I2C_Mem_Write");
}

void PA_RegisterWrite32(uint8_t Index, uint32_t Value)
{
  uint8_t Data[4];

  Data[0] = (uint8_t)(Value >> 24);
  Data[1] = (uint8_t)(Value >> 16);
  Data[2] = (uint8_t)(Value >> 8);
  Data[3] = (uint8_t)(Value);

  HandleHALResult(HAL_I2C_Mem_Write(&hi2c1, PA_I2C_Address << 1, Index, 1, Data, 4, 1000), "HAL_I2C_Mem_Write");
}

void PA_Initialize(uint8_t HeadphoneMode)
{
  // Power up:
  HAL_GPIO_WritePin(TAS_PDN_GPIO_Port, TAS_PDN_Pin, 0);
  HAL_GPIO_WritePin(TAS_NRST_GPIO_Port, TAS_NRST_Pin, 0);
  HAL_Delay(1);
  HAL_GPIO_WritePin(TAS_PDN_GPIO_Port, TAS_PDN_Pin, 1);
  HAL_Delay(1);
  HAL_GPIO_WritePin(TAS_NRST_GPIO_Port, TAS_NRST_Pin, 1);
  HAL_Delay(10); // ???

  // Tune output PWM to prevent slight audible whining due to clash with switch mode PSU on JSB living room system.
  // The default value is 6, which gives a frequency of ~360 kHz.
  // A value of 7 gives a frequency of ~329 kHz.
  PA_RegisterWrite32(0x4F, 0x00000007);

  PA_RegisterWrite8(0x1B, 0x00);
  PA_RegisterWrite8(0x00, 0x6C);

  if (HeadphoneMode)
  {
    PA_RegisterWrite8(0x05, 0x01); // !!! Is this necessary?
    PA_RegisterWrite8(0x05, 0x13); // 0x13 or 0x1B. But 0x1B gives background noise and distortion.
  }
  else
    PA_RegisterWrite8(0x05, 0x00);

  PA_Mute();
}

uint8_t PA_GetErrorStatus()
{
  return PA_RegisterRead8(0x02);
}

void PA_ClearErrorStatus()
{
  PA_RegisterWrite8(0x02, 0x00);
}

void PA_SetMasterVolume(float Volume_dB)
{
  uint16_t RegisterValue;

  if (Volume_dB > 24)
    Volume_dB = 24;
  if (Volume_dB < -103.75)
    Volume_dB = -103.75;

  RegisterValue = 8 * (24 - Volume_dB);

  PA_RegisterWrite16(0x07, RegisterValue);
}

void PA_Mute()
{
  PA_RegisterWrite16(0x07, 0x03FF);
}

#define ReverseBytes(X) ((X << 24) | (( X & 0xFF00 ) << 8) | (( X >> 8) & 0xFF00 ) | ( X >> 24 ))

void PA_ReadPWMLevelMeter(uint32_t *pLeft, uint32_t *pRight)
{
  uint32_t Value_ByteReversed;

  HandleHALResult(HAL_I2C_Mem_Read(&hi2c1, PA_I2C_Address << 1, 0x6B, 1, (uint8_t *)&Value_ByteReversed, 4, 1000), "HAL_I2C_Mem_Read");
  *pLeft = ReverseBytes(Value_ByteReversed);
  HandleHALResult(HAL_I2C_Mem_Read(&hi2c1, PA_I2C_Address << 1, 0x6C, 1, (uint8_t *)&Value_ByteReversed, 4, 1000), "HAL_I2C_Mem_Read");
  *pRight = ReverseBytes(Value_ByteReversed);
}

float PA_Convert323FormatToFloat(uint32_t Value)
// Not tested!
{
  uint8_t Negative;
  int8_t Power;
  int32_t Mask;
  float BitValue, Result;

  Value &= 0X03FFFFFF;

  Negative = 0;

  if (Value & 0X02000000)
  {
    Value = ~Value + 1;
    Value &= 0X03FFFFFF;

    Negative = 1;
  }

  Result = 0.0f;
  Mask = 0x01000000;
  BitValue = 1.0f;
  for (Power = 1; Power >= -23; --Power)
  {
    if (Value & Mask)
      Result += BitValue;

    Mask = Mask >> 1;
    BitValue = 0.5f * BitValue;
  }

  if (Negative)
    Result = -Result;

  return Result;
}

///////////////////////////////////////////////////////////////////////////////
// ESP32s:

void ESP32_A_Enable(uint8_t Value)
{
  HAL_GPIO_WritePin(ESP32_A_EN_GPIO_Port, ESP32_A_EN_Pin, Value);
}

void ESP32_B_Enable(uint8_t Value)
{
  HAL_GPIO_WritePin(ESP32_B_EN_GPIO_Port, ESP32_B_EN_Pin, Value);
}

///////////////////////////////////////////////////////////////////////////////
// RGBs (SK6812):

static uint16_t *RGB_pData = NULL;
static uint16_t RGB_pData_NumItems = 0;
static uint16_t RGB_Number;
static uint16_t RGB_DataIndex;

// EITHER: 65 (0.3 us) & 130 (0.6 us).
// OR: 75 (0.35 us) & 150 (0.7 us).
#define RGB_NumClockCycles_Low 65
#define RGB_NumClockCycles_High 130
#define RGB_NumPWMCyclesForReset 40

void RGB_SetNumber(uint8_t Value)
{
  uint16_t NumPWMCycles;

  if (RGB_pData)
  {
    free(RGB_pData);
    RGB_pData = NULL;
    RGB_Number = 0;
  }

  if (Value)
  {
    NumPWMCycles = RGB_NumPWMCyclesForReset + 3 * 8 * Value + 1;
    RGB_pData_NumItems = NumPWMCycles;
    RGB_pData = calloc(RGB_pData_NumItems, 2);
    RGB_Number = Value;
  }
}

void RGB_Begin()
{
  uint8_t Index;

  RGB_DataIndex = 0;

  for (Index = 0; Index < RGB_NumPWMCyclesForReset; ++Index)
  {
    if (RGB_DataIndex >= RGB_pData_NumItems)
      Error_Handler();
    RGB_pData[RGB_DataIndex++] = 0;
  }
}

static void RGB_AppendColourComponent(uint8_t ColorComponent)
{
  uint8_t Remainder;

  Remainder = ColorComponent;
  for (uint8_t BitIndex=0; BitIndex<8;++BitIndex)
  {
    if (RGB_DataIndex == RGB_Number)
    {
      HandleError();
      return;
    }

    if (RGB_DataIndex >= RGB_pData_NumItems)
      Error_Handler(); // Prevent memory corruption caused by writing outside the array.

    RGB_pData[RGB_DataIndex++] = Remainder & 0x80? RGB_NumClockCycles_High : RGB_NumClockCycles_Low;
    Remainder = Remainder << 1;
  }
}

void RGB_AppendColour(uint32_t Color)
{
  RGB_AppendColourComponent(Color >> 8);
  RGB_AppendColourComponent(Color);
  RGB_AppendColourComponent(Color >> 16);
}

void RGB_End()
{
  HAL_StatusTypeDef HALStatus;

  RGB_pData[RGB_DataIndex++] = 0;
  // It might be better to leave the line high, to reduce power dissipation in the BS170 / 1K voltage level shifter.
  // However, it doesn't work reliably...
  //RGBData[RGBDataIndex++] = 270;

  if (RGB_DataIndex != RGB_pData_NumItems) // Check that the array was the correct size.
    Error_Handler();

  HALStatus = HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)RGB_pData, RGB_DataIndex);
  if (HALStatus != HAL_OK)
    Error_Handler();

  do {} while (htim1.hdma[TIM_DMA_ID_CC1]->State == HAL_DMA_STATE_BUSY);
}

///////////////////////////////////////////////////////////////////////////////
// Test points:
//
void TestPoint_Set(uint8_t Value)
{
  HAL_GPIO_WritePin(TestPoint_GPIO_Port, TestPoint_Pin, Value);
}

///////////////////////////////////////////////////////////////////////////////
