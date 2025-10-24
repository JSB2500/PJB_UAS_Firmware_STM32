#include <math.h>
#include "main.h"
#include "PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.h"
#include "JSB_IR_RC5.h"

///////////////////////////////////////////////////////////////////////////////
// Notes:
// => Requires an IR sensor.
//    => E.g. TSOP4838:
//       => Pins numbered 1 to 3 from left to right facing device with legs pointing downward.
//          => Pin 1: Out.
//          => Pin 2: 0V.
//          => Pin 3: 3V3.
//
// => Requires a timer e.g. Timer 3 (Tim3).
//    => Connect IR sensor output to timer channel 1 input.
//    => Set "Slave Mode" to "Reset Mode".
//    => Set "Trigger Source" to "TI1FP1".
//    => Set "Clock Source" to "Internal Clock".
//    => Counter:
//       => Configure prescaler to give a timer clock of 1 MHz e.g. a value of 107 for a timer clock frequency of 108 MHz.
//    => Channel 1:
//       => Set "Channel 1" to "Input Capture direct mode".
//       => Set "Polarity Selection" to "Both Edges".
//       => Set "Prescaler Division Ratio" to "No division".
//       => Set "Input Filter" to "0".
//   => Interrupt:
//      => Enable global interrupt.
// => Requires USE_HAL_TIM_REGISTER_CALLBACKS symbol to be defined as 1 e.g. add "USE_HAL_TIM_REGISTER_CALLBACKS=1" to C compiler symbols in "Tool Settings".
///////////////////////////////////////////////////////////////////////////////

static uint8_t IsCloseTo_f(float ActualValue, float IntendedValue, float AllowedError)
{
  return fabs(ActualValue - IntendedValue) <= AllowedError;
}

///////////////////////////////////////////////////////////////////////////////

#define Decoder_MaxNumBits 32

typedef enum
{
  stNone,
  stReceiving,
  stReceived,
  stDone
} Decoder_State_t;

static TIM_HandleTypeDef *phTimer = NULL;
static uint32_t Decoder_NumBits = 0;
static Decoder_State_t Decoder_State = stNone;
static uint8_t Decoder_CurrentInputState = 0;
static uint8_t Decoder_WasMidBit = 0;
static uint8_t Decoder_Error = 0;
static uint32_t Decoder_Bits = 0;

///////////////////////////////////////////////////////////////////////////////

static void PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == phTimer)
  {
    if (Decoder_State == stDone)
    {
      Decoder_State = stNone;
    }
    else if (Decoder_State == stReceiving)
      Decoder_State = stReceived;
  }
}

static void CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim == phTimer)
  {
    if (Decoder_State == stNone)
    {
      Decoder_NumBits = 0;
      Decoder_State = stReceiving;
      Decoder_WasMidBit = 1;
      Decoder_CurrentInputState = 0;
      Decoder_Bits = 0;
      Decoder_Error = 0;
    }
    else if (Decoder_State == stReceiving)
    {
      if (Decoder_NumBits == Decoder_MaxNumBits)
        Decoder_State = stDone;
      else
      {
        uint8_t NumHalfCycles = (int)round(HAL_TIM_ReadCapturedValue(phTimer, TIM_CHANNEL_1) / 889.0f); // Assumes channel 1.

        if (Decoder_WasMidBit && (NumHalfCycles == 1))
        {
          Decoder_WasMidBit = 0;
        }
        // Expect mid-bit here.
        else if ((Decoder_WasMidBit && (NumHalfCycles == 2)) || (!Decoder_WasMidBit && (NumHalfCycles == 1)))
        {
          Decoder_WasMidBit = 1;
          Decoder_Bits = (Decoder_Bits << 1) | Decoder_CurrentInputState;
          ++Decoder_NumBits;
        }
        else
        {
          Decoder_Error = 1;
          Decoder_State = stReceived;
        }
      }

      Decoder_CurrentInputState = !Decoder_CurrentInputState;
    }
  }
}

void JSB_IR_RC5_Initialize(TIM_HandleTypeDef *i_phTimer)
{
  phTimer = i_phTimer;

  // Check that the timer frequency is set correctly:
  float TimerCountFrequency = (float)GetAPB1TimerClockFrequency() / ((float)phTimer->Instance->PSC + 1.0f);
  if (!IsCloseTo_f(TimerCountFrequency / 1E6, 1.0f, 0.01f))
    Error_Handler();

  if (HAL_TIM_Base_Start_IT(phTimer))
    Error_Handler();
  if (HAL_TIM_IC_Start_IT(phTimer, TIM_CHANNEL_1)) // Assumes channel 1.
    Error_Handler();

  __HAL_TIM_URS_ENABLE(phTimer); // Prevent HAL_TIM_PeriodElapsedCallback() being called when counter is reset by input.

  HAL_TIM_RegisterCallback(phTimer, HAL_TIM_PERIOD_ELAPSED_CB_ID, &PeriodElapsedCallback);
  HAL_TIM_RegisterCallback(phTimer, HAL_TIM_IC_CAPTURE_CB_ID, &CaptureCallback);
}

uint8_t JSB_IR_RC5_GetIfAvailable(uint8_t *o_Address, uint8_t *o_Command, uint8_t *o_Toggle, uint8_t *o_Error)
{
  if (Decoder_State != stReceived)
    return 0;

  *o_Address = (Decoder_Bits >> 6) & 0x1F;
  *o_Command = Decoder_Bits & 0x3F;
  *o_Toggle = (Decoder_Bits >> 11) & 0x01;
  *o_Error = Decoder_Error;
  Decoder_State = stDone;
  return 1;
}

///////////////////////////////////////////////////////////////////////////////
