// Copyright 2018 J S Bladen.
///////////////////////////////////////////////////////////////////////////////

#ifndef __JSB_UI_Elements_H
#define __JSB_UI_Elements_H

#include "JSB_ILI9341.h"
#include "JSB_XPT2046.h"
#include "gfxfont.h"
#include "FreeSans9pt7b.h"
#include "FreeSans12pt7b.h"

#define JSB_UI_DisplayWidth (240)
#define JSB_UI_DisplayHeight (320)
#define JSB_UI_Control_MaxTextLength (80)
#define JSB_UI_Controls_MaxNumber (16)

typedef struct
{
  uint32_t Left;
  uint32_t Top;
  uint32_t Width;
  uint32_t Height;
} JSB_UI_Rect_t;

typedef enum
{
  ctNone,
  ctButton,
  ctHorizontalSlider,
  ctVerticalSlider
} JSB_UI_ControlType_t;

typedef struct JSB_UI_Control_t JSB_UI_Control_t; // Forward declaration to support using pointer to self in self.

struct JSB_UI_Control_t
{
  JSB_UI_ControlType_t Type;
  JSB_UI_Rect_t Rect;
  char Text[JSB_UI_Control_MaxTextLength];
  uint16_t ForegroundColor;
  uint16_t BackgroundColor;
  float *pSliderValue;
  float SliderMinValue;
  float SliderMaxValue;
  uint8_t Visible;
  void (*pTweakTextHandler)(JSB_UI_Control_t *pControl, char *Result, uint32_t MaxResultLength);
  void (*pPressDownHandler)(JSB_UI_Control_t *);
  void (*pPressMoveHandler)(JSB_UI_Control_t *);
  void (*pCustomDrawHandler)(JSB_UI_Control_t *);
};

typedef struct
{
  uint32_t NumControls;
  JSB_UI_Control_t *pControls[JSB_UI_Controls_MaxNumber];
} JSB_UI_Controls_t;

typedef struct JSB_UI_Page_t JSB_UI_Page_t; // Forward declaration to support using pointer to self in self.

struct JSB_UI_Page_t
{
  JSB_UI_Controls_t Controls;
  uint8_t ContinuousRefresh;
  void (*pCustomDrawHandler)(JSB_UI_Page_t *);
};

JSB_UI_Rect_t JSB_UI_Rect(uint32_t Left, uint32_t Top, uint32_t Width, uint32_t Height);
JSB_UI_Rect_t JSB_UI_RectFromCentrePoint(uint32_t CentreX, uint32_t CentreY, uint32_t Width, uint32_t Height);
//
void JSB_UI_InitializeDisplay();
JSB_UI_Page_t *JSB_UI_CreatePage();
JSB_UI_Control_t *JSB_UI_CreateControl(JSB_UI_ControlType_t ControlType, JSB_UI_Rect_t Rect, char *Text, uint16_t ForegroundColor, uint16_t BackgroundColor,
  float *pSliderValue,
  uint8_t Visible,
  void (*pPressDownHandler)(JSB_UI_Control_t *), void (*pPressMoveHandler)(JSB_UI_Control_t *), void (*pCustomDrawHandler)(JSB_UI_Control_t *));
uint8_t JSB_UI_Control_IsSlider(JSB_UI_Control_t *pControl);
void JSB_UI_AddControlToPage(JSB_UI_Page_t *pPage, JSB_UI_Control_t *pControl);
void JSB_UI_DrawPage(JSB_UI_Page_t *pPage);
uint8_t JSB_UI_Page_PressDown(JSB_UI_Page_t *pPage, int16_t X, int16_t Y); // NB: (X,Y) must be signed.
void JSB_UI_Page_PressMove(JSB_UI_Page_t *pPage, int16_t X, int16_t Y); // NB: (X,Y) must be signed.
void JSB_UI_Page_PressUp();

#endif
