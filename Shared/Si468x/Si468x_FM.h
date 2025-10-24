#ifndef __SI468X_FM_H
#define __SI468X_FM_H

typedef struct
{
  float Frequency;
  int8_t RSSI; // -128 to 127 dBuV
  int8_t SNR; // -128 to 127 dB
  uint8_t MultiPath; // 0 to 255
} JSB_FM_RSQ_Status_t;

void si468x_FM_tune(float MHz);
void si468x_FM_seek(uint8_t up, uint8_t wrap);
void si468x_FM_RDS_status();
void JSB_si468x_FM_get_rsq_status(JSB_FM_RSQ_Status_t *pStatus);
float JSB_si468x_FM_get_frequency();
uint8_t JSB_si468x_FM_get_stereo_separation();

#endif
