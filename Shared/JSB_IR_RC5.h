///////////////////////////////////////////////////////////////////////////////
// Copyright 2018 J S Bladen.
///////////////////////////////////////////////////////////////////////////////

#ifndef __JSB_IR_RC5_H
#define __JSB_IR_RC5_H

#include <stdint.h>
#include <stm32f7xx_hal.h>

void JSB_IR_RC5_Initialize(TIM_HandleTypeDef *i_phTimer);
uint8_t JSB_IR_RC5_GetIfAvailable(uint8_t *o_Address, uint8_t *o_Command, uint8_t *o_Toggle, uint8_t *o_Error);

#endif

///////////////////////////////////////////////////////////////////////////////
// Humax F01 remote control.
/////////
// Type: MDB 1.3 (3 digit codes). Audio mode: Configuration code 043:
//
// Button Address Command:
//
// Mute 16 13
// Power 16 12
// ---
// 1 17 63
// 2 12 63
// 3 20 63
// 4 5 63
// 5 -
// 6 0 63
// 7 22 63
// 8 18 63
// 9 -
// 0 4 63
// ---
// Play 20 53
// Pause 20 48
// Stop 20 54
// Record -
// ---
// Left 20 50
// Right 20 52
// LeftBar 20 33
// RightBar 20 32
// ---
// "Back" 20 59
// ---
// Volume 16 17 16
// "i" ("Info") 17 15
// "Opt+" 20 28
// Channel 17 33 32
// ---
// "Speaker?" 16 63
// "TV/Radio" 17 46
// "Source" 16 59
// "Sleep" 16 38
/////////
// Type: ADB 1.0 (4 digit codes). Audio mode: Configuration code 01189: Seems to work with this software.
///////////////////////////////////////////////////////////////////////////////
