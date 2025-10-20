
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "spi.h"
#include "JSB_General.h"
#include "JSB_UI_Elements.h"
#include "PJB_STM32_ESP32_FM_DAB_ADC_DAC_PA.h"

static JSB_UI_Control_t *pPressedControl = NULL;

///////////////////////////////////////////////////////////////////////////////
// Minor standalone support functions:

static uint8_t HitRect(uint16_t X, uint16_t Y, JSB_UI_Rect_t *pRect)
{
  if ((X < pRect->Left) || (X >= pRect->Left + pRect->Width))
    return 0;
  if ((Y < pRect->Top) || (Y >= pRect->Top + pRect->Height))
    return 0;
  return 1;
}

float Interpolate_float(float MinValue, float MaxValue, float Parameter)
{
  return (1.0f - Parameter) * MinValue + Parameter * MaxValue;
}

///////////////////////////////////////////////////////////////////////////////

static void DrawControls(JSB_UI_Page_t *pPage)
{
  uint32_t ControlIndex;
  JSB_UI_Control_t *pControl;
  JSB_UI_Rect_t Rect;
  uint16_t ForegroundColor, BackgroundColor;

  TextDrawMode_t OriginalTextDrawMode = ILI9341_SetTextDrawMode(tdmMergeWithExistingPixels);
  TextVerticalOrigin_t OriginalTextVerticalOrigin = ILI9341_SetTextVerticalOrigin(tvoCentreBetweenBaseAndTop);

  for (ControlIndex = 0; ControlIndex < pPage->Controls.NumControls; ++ControlIndex)
  {
    pControl = pPage->Controls.pControls[ControlIndex];
    Rect = pControl->Rect;
    ForegroundColor = pControl->ForegroundColor;
    BackgroundColor = pControl->BackgroundColor;

    if (JSB_UI_Control_IsSlider(pControl))
    {
      float SliderParameter = (*pControl->pSliderValue - pControl->SliderMinValue) / (pControl->SliderMaxValue - pControl->SliderMinValue);

      if (pControl->Type == ctHorizontalSlider)
      {
        uint16_t OnWidth = round(Rect.Width * SliderParameter);
        uint16_t OffWidth = Rect.Width - OnWidth;
        ILI9341_DrawBar(Rect.Left, Rect.Top, OnWidth, Rect.Height, ForegroundColor);
        ILI9341_DrawBar(Rect.Left + OnWidth, Rect.Top, OffWidth, Rect.Height, BackgroundColor);
      }
      else if (pControl->Type == ctVerticalSlider)
      {
        uint16_t OnHeight = round(Rect.Width * SliderParameter);
        uint16_t OffHeight = Rect.Height - OnHeight;
        ILI9341_DrawBar(Rect.Left, Rect.Top, Rect.Width, OnHeight, ForegroundColor);
        ILI9341_DrawBar(Rect.Left, Rect.Top + OnHeight, Rect.Width, OffHeight, BackgroundColor);
      }
    }
    else
      ILI9341_DrawBar(Rect.Left, Rect.Top, Rect.Width, Rect.Height, BackgroundColor);

    if (pControl->pCustomDrawHandler)
      (*pControl->pCustomDrawHandler)(pControl);

    char Text[JSB_UI_Control_MaxTextLength];
    char *pText;

    if (pControl->pTweakTextHandler)
    {
      (*pControl->pTweakTextHandler)(pControl, Text, JSB_UI_Control_MaxTextLength);
      pText = Text;
    }
    else
      pText = pControl->Text;

    ILI9341_DrawTextAtXY(pText, pControl->Rect.Left + ((pControl->Rect.Width - ILI9341_GetTextWidth(pText)) / 2), pControl->Rect.Top + (pControl->Rect.Height / 2), tpLeft);
  }

  ILI9341_SetTextDrawMode(OriginalTextDrawMode);
  ILI9341_SetTextVerticalOrigin(OriginalTextVerticalOrigin);
}

void JSB_UI_Page_UpdateSlideValue(JSB_UI_Control_t *pControl, int16_t X, int16_t Y)
{
  float SliderParameter;

  if (pControl->Type == ctHorizontalSlider)
    SliderParameter = pControl->Rect.Width == 0 ? 0 : Clamp_float(((float)X - (float)pControl->Rect.Left) / (float)pControl->Rect.Width, 0.0f, 1.0f);
  else if (pControl->Type == ctVerticalSlider)
    SliderParameter = pControl->Rect.Height == 0 ? 0 : Clamp_float(((float)Y - (float)pControl->Rect.Top) / (float)pControl->Rect.Height, 0.0f, 1.0f);
  else return;

  *pControl->pSliderValue = Interpolate_float(pControl->SliderMinValue, pControl->SliderMaxValue, SliderParameter);
}

uint8_t JSB_UI_Page_PressDown(JSB_UI_Page_t *pPage, int16_t X, int16_t Y)
{
  uint32_t ControlIndex;
  JSB_UI_Control_t *pControl;
  JSB_UI_Rect_t Rect;

  if (pPressedControl)
    return 0;

  for (ControlIndex = 0; ControlIndex < pPage->Controls.NumControls; ++ControlIndex)
  {
    pControl = pPage->Controls.pControls[ControlIndex];
    Rect = pControl->Rect;

    if (HitRect(X, Y, &Rect))
    {
      pPressedControl = pControl;

      if (JSB_UI_Control_IsSlider(pControl))
        JSB_UI_Page_UpdateSlideValue(pControl, X, Y);

      if (pControl->pPressDownHandler)
        (*pControl->pPressDownHandler)(pControl);

      return 1;
    }
  }

  return 0;
}

void JSB_UI_Page_PressMove(JSB_UI_Page_t *pPage, int16_t X, int16_t Y)
{
  if (!pPressedControl)
    return;

  if ((pPressedControl->Type == ctHorizontalSlider) || (pPressedControl->Type == ctVerticalSlider))
    JSB_UI_Page_UpdateSlideValue(pPressedControl, X, Y);

  if (pPressedControl->pPressMoveHandler)
    (*pPressedControl->pPressMoveHandler)(pPressedControl);
}

void JSB_UI_Page_PressUp()
{
  pPressedControl = NULL;
}

JSB_UI_Page_t *JSB_UI_CreatePage()
{
  return calloc(1, sizeof(JSB_UI_Page_t));
}

JSB_UI_Rect_t JSB_UI_Rect(uint32_t Left, uint32_t Top, uint32_t Width, uint32_t Height)
{
  JSB_UI_Rect_t Rect;

  Rect.Left = Left;
  Rect.Top = Top;
  Rect.Width = Width;
  Rect.Height = Height;

  return Rect;
}

JSB_UI_Rect_t JSB_UI_RectFromCentrePoint(uint32_t CentreX, uint32_t CentreY, uint32_t Width, uint32_t Height)
{
  JSB_UI_Rect_t Rect;

  Rect.Left = CentreX - Width / 2;
  Rect.Top = CentreY - Height / 2;
  Rect.Width = Width;
  Rect.Height = Height;

  return Rect;
}

JSB_UI_Control_t *JSB_UI_CreateControl(JSB_UI_ControlType_t ControlType, JSB_UI_Rect_t Rect, char *Text, uint16_t ForegroundColor, uint16_t BackgroundColor,
  float *pSliderValue,
  uint8_t Visible,
  void (*pPressDownHandler)(JSB_UI_Control_t *), void (*pPressMoveHandler)(JSB_UI_Control_t *), void (*pCustomDrawHandler)(JSB_UI_Control_t *))
{
  JSB_UI_Control_t *pControl;

  pControl = calloc(1, sizeof(JSB_UI_Control_t));

  pControl->Type = ControlType;
  pControl->Rect = Rect;
  strncpy(pControl->Text, Text, JSB_UI_Control_MaxTextLength);
  pControl->ForegroundColor = ForegroundColor;
  pControl->BackgroundColor = BackgroundColor;
  pControl->pSliderValue = pSliderValue;
  pControl->SliderMinValue = 0.0f;
  pControl->SliderMaxValue = 1.0f;
  pControl->Visible = Visible;
  pControl->pPressDownHandler = pPressDownHandler;
  pControl->pPressMoveHandler = pPressMoveHandler;
  pControl->pCustomDrawHandler = pCustomDrawHandler;

  return pControl;
}

uint8_t JSB_UI_Control_IsSlider(JSB_UI_Control_t *pControl)
{
  return (pControl->Type == ctHorizontalSlider) || (pControl->Type == ctVerticalSlider);
}

void JSB_UI_AddControlToPage(JSB_UI_Page_t *pPage, JSB_UI_Control_t *pControl)
{
  if (pPage->Controls.NumControls == JSB_UI_Controls_MaxNumber)
  {
    Error_Handler();
    return;
  }

  pPage->Controls.pControls[pPage->Controls.NumControls] = pControl;
  pPage->Controls.NumControls += 1;
}

void JSB_UI_SetDefaultState()
{
  ILI9341_SetDefaultState();

  ILI9341_SetFont(&FreeSans9pt7b);
}

void JSB_UI_InitializeDisplay()
{
  ILI9341_Initialize(&hspi1, 1);
  LCD_BacklightOn(1);
  XPT2046_Initialize(&hspi2);

  JSB_UI_SetDefaultState();

  ILI9341_Clear(0x0000);
}

void JSB_UI_DrawPage(JSB_UI_Page_t *pPage)
{
  ILI9341_Clear(0);

  DrawControls(pPage);

  if (pPage->pCustomDrawHandler)
    (*pPage->pCustomDrawHandler)(pPage);

  if (ILI9341_UsingBackBuffer())
    ILI9341_CopyBackBufferToDisplay();
}
