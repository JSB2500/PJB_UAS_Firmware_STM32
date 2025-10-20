#ifndef JSB_SPDIFRX_H_
#define JSB_SPDIFRX_H_

#include "JSB_General.h"

#define InputSampleFrequencyCaptureHalfRangePPM 1000 /* The required (IEC 60958-3 Level II) pull-in range is +/-1000 ppm e.g. +/- 48 for a sample rate of 48000Hz. */

// Buffer size is a tradeoff between acceptably low latency and acceptably low jitter.
// Increase when sending serial debugging information. (And use a fast baud rate e.g. 921600).
#define SPDIFRX_Buffer_NumSamples (640)

extern float SPDIFRX_TransferGain;

void SPDIFRX_SelectInput(uint32_t Value);
uint32_t SPDIFRX_GetInputSampleRate();
uint8_t SPDIFRX_IsStarted();
uint8_t SPDIFRX_Start(float TargetSampleRate, uint32_t InputChannel, uint8_t SPDIFRX_ForceOutputSampleRate, uint32_t SPDIFRX_ForcedOutputSampleRate);
void SPDIFRX_Stop();
void SPDIFRX_CopySamplesToGivenBuffer(AudioSample24_t *pOutputBuffer, AudioBufferHalf_t AudioBufferHalf);
uint8_t SPDIFRX_HasReceiveErrorOccurred();
uint8_t SPDIFRX_IsReceiving();
void SPDIFRX_GetNumErrors(uint8_t *pNumTransitionErrors, uint8_t *pNumSyncErrors, uint8_t *pNumFrameErrors);

#endif
