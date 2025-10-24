#include <stdlib.h>
#include "Si468x/Si468x.h"
#include "Si468x/Si468x_FM.h"

// FM:
#define FM_TUNE_FREQ				0x30
#define FM_SEEK_START				0x31
#define FM_RSQ_STATUS				0x32
#define FM_ACF_STATUS       0x33
#define FM_RDS_STATUS				0x34
#define FM_RDS_BLOCKCOUNT			0x35

void si468x_FM_tune(float MHz)
{
	if (JSB_si468x_get_current_mode() != Si468x_MODE_FM)
		return;

	uint16_t freq = MHz * 100;
	uint8_t args[] = {0x00, freq & 0xFF, freq >> 8, 0x00, 0x00};
	Si468x_Command *command = si468x_build_command(FM_TUNE_FREQ, args, 5);
	Interrupt_Status.STCINT = 0;
	si468x_execute(command);
	si468x_wait_for_interrupt(STCINT);
	si468x_free_command(command);
}

void si468x_FM_seek(uint8_t up, uint8_t wrap)
{
	if (JSB_si468x_get_current_mode() != Si468x_MODE_FM)
		return;

	uint8_t args[] = {
			0x10,
			((up & 0x1) << 1) | (wrap & 0x1),
			0x00,
			0x00,
			0x00
	};
	Si468x_Command *command = si468x_build_command(FM_SEEK_START, args, 5);
	Interrupt_Status.STCINT = 0;
	si468x_execute(command);
	free(command);
	si468x_wait_for_interrupt(STCINT);
}

unsigned char station_name[8];
void si468x_FM_RDS_status()
{
	uint8_t args[] = {0x01};
	Si468x_Command *command = si468x_build_command(FM_RDS_STATUS, args, 1);
	si468x_execute(command);
	si468x_free_command(command);

	uint8_t rds_data[20];
	si468x_read_response(rds_data, 20);

	//!!! uint16_t BLOCK_A = ((uint16_t) rds_data[13] << 8) + rds_data[12];
	uint16_t BLOCK_B = ((uint16_t) rds_data[15] << 8) + rds_data[14];
	uint16_t BLOCK_C = ((uint16_t) rds_data[17] << 8) + rds_data[16];
	uint16_t BLOCK_D = ((uint16_t) rds_data[19] << 8) + rds_data[18];
	uint8_t group_type = (BLOCK_B & 0xF800) >> 8; //!!!
	uint8_t C10 = BLOCK_B & 0x03;
	if (group_type == 0)
	{
		uint8_t decoder_identification_code = 3 - C10;
		station_name[6 - 2 * decoder_identification_code] = BLOCK_D >> 8;
		station_name[6 - 2 * decoder_identification_code + 1] = BLOCK_D & 0xFF;
	}
	else if (group_type == 4)
	{
		uint32_t date_and_time;
		date_and_time = (BLOCK_B & 0x03) << 16;
		date_and_time += BLOCK_C << 8;
		date_and_time += BLOCK_D;

		// PJB!!! uint8_t minutes = (date_and_time & 0x0FC0) >> 5;
	}
}

void JSB_si468x_FM_get_rsq_status(JSB_FM_RSQ_Status_t *pStatus)
{
  uint8_t read_buffer[22];

  if (JSB_si468x_get_current_mode() != Si468x_MODE_FM)
    return;

  uint8_t args[] = {0x00};
  Si468x_Command *pCommand = si468x_build_command(FM_RSQ_STATUS, args, 1);
  si468x_execute(pCommand);
  si468x_read_response(read_buffer, 22);
  pStatus->Frequency = 0.01f * (read_buffer[6] + (((uint16_t) read_buffer[7]) << 8));
  pStatus->RSSI = (int8_t)read_buffer[9];
  pStatus->SNR = (int8_t)read_buffer[10];
  pStatus->MultiPath = read_buffer[11];
  si468x_free_command(pCommand);
}

float JSB_si468x_FM_get_frequency()
{
  JSB_FM_RSQ_Status_t Status;
  JSB_si468x_FM_get_rsq_status(&Status);
  return Status.Frequency;
}

uint8_t JSB_si468x_FM_get_stereo_separation()
{
  uint8_t read_buffer[10];
  uint8_t StereoSeparation;

  if (JSB_si468x_get_current_mode() != Si468x_MODE_FM)
    return 0;

  uint8_t args[] = {0x00};
  Si468x_Command *pCommand = si468x_build_command(FM_ACF_STATUS, args, 1);
  si468x_execute(pCommand);
  si468x_read_response(read_buffer, 10);
  StereoSeparation = read_buffer[8] & 0x7F;
  si468x_free_command(pCommand);

  return StereoSeparation;
}
