// Written by P J Bladen 2018.

#include "PJB_SST25V.h"
#include "gpio.h"
#include "spi.h"

#define FLASH_READ_SLOW	0x03
#define FLASH_READ_FAST	0x0B
#define FLASH_WRITE_ENABLE 0x06
#define FLASH_WRITE_DISABLE 0x04
#define FLASH_ERASE_4K 0x20
#define FLASH_AAI 0xAD
#define FLASH_BYTE_PROGRAM 0x02
#define FLASH_ENABLE_SO 0x70
#define FLASH_DISABLE_SO 0x80
#define FLASH_ENABLE_WRITE_STATUS	0x50
#define FLASH_WRITE_STATUS 0x01
#define FLASH_READ_STATUS	0x05

#define FLASH_SBIT_BUSY 0x01
#define FLASH_SBIT_WRITE_ENABLE 0x01

#define DWT_CYCCNT ((volatile uint32_t *)0xE0001004)
#define CPU_CYCLES (*DWT_CYCCNT)

static void SST25_Start();
static void SST25_End();
static uint8_t SST25_ReadStatus();
static void SST25_EnableHardware_EOW();
static void SST25_WriteEnable();
static void SST25_WriteStatus(uint8_t status);
static void SST25_Delay();

///////////////////////////////////////////////////////////////////////////////
// These must be provided (hardware specific):

extern void SST25_SPI_WriteBlock(uint8_t *pBlock, uint16_t NumBytes);
extern void SST25_SPI_ReadBlock(uint8_t *pBlock, uint16_t NumBytes);
extern uint8_t SST25_SPI_ReadWriteByte(uint8_t Value);
extern void SST25_SPI_Begin();
extern void SST25_SPI_End();

///////////////////////////////////////////////////////////////////////////////

void SST25_EraseSector_4K(uint32_t Address)
{
	Address &= 0xFFFFFF;

	SST25_WriteStatus(0x00); // Clear all sector protection
	SST25_WriteEnable();

	SST25_Start();
	uint8_t cmd[] =
	{
			FLASH_ERASE_4K,
			(Address >> 16) & 0xFF,
			(Address >> 8) & 0xFF,
			Address & 0xFF
	};
	SST25_SPI_WriteBlock(cmd, 4);
	SST25_End();

	while (SST25_ReadStatus() & FLASH_SBIT_BUSY);
}

void SST25_EnableHardware_EOW()
{
	SST25_Start();
	SST25_SPI_ReadWriteByte(FLASH_ENABLE_SO);
	SST25_End();
}

void SST25_WriteEnable()
{
	SST25_Start();
	SST25_SPI_ReadWriteByte(FLASH_WRITE_ENABLE);
	SST25_End();
}

void SST25_WriteStatus(uint8_t Status)
{
	SST25_Start();
	SST25_SPI_ReadWriteByte(FLASH_ENABLE_WRITE_STATUS);
	SST25_End();

	SST25_Start();
	uint8_t cmd[] = {FLASH_WRITE_STATUS, Status};
	SST25_SPI_WriteBlock(cmd, 2);
	SST25_End();
}

uint8_t SST25_ReadStatus()
{
	HAL_StatusTypeDef HAL_Result;

	SST25_Start();
	uint8_t Command[] = {FLASH_READ_STATUS, 0x00};
	uint8_t Received[2];

	if ((HAL_Result = HAL_SPI_TransmitReceive(&hspi3, Command, Received, 2, 1000)) != HAL_OK)
		Error_Handler();

	SST25_End();
	return Received[1];
}

void SST25_WriteByte(uint32_t Address, uint8_t Data)
{
	if (!(SST25_ReadStatus() & FLASH_SBIT_WRITE_ENABLE))
		SST25_WriteEnable();

	Address &= 0xFFFFFF;

	SST25_Start();
	uint8_t cmd[] =
	{
			FLASH_BYTE_PROGRAM,
			(Address >> 16) & 0xFF,
			(Address >> 8) & 0xFF,
			Address & 0xFF,
			Data
	};
	SST25_SPI_WriteBlock(cmd, 5);
	SST25_End();
}

void SST25_WriteBlock(uint32_t Address, uint8_t *pBlock, uint16_t NumBytes)
{
	if (!(SST25_ReadStatus() & FLASH_SBIT_WRITE_ENABLE))
		SST25_WriteEnable();

	SST25_EnableHardware_EOW();

	Address &= 0xFFFFFF;
	uint8_t Address_cmd[] =
	{
			FLASH_AAI,
			(Address >> 16) & 0xFF,
			(Address >> 8) & 0xFF,
			Address & 0xFF,
	};
	uint8_t DataCommand[3] = {FLASH_AAI, 0x00, 0x00};
	uint16_t NumBytesWritten = 0;
	uint16_t NumBytesRemaining;

	while ((NumBytesRemaining = NumBytes - NumBytesWritten) > 1)
	{
		SST25_Start();
		if (!NumBytesWritten)
			SST25_SPI_WriteBlock(Address_cmd, 4);

		DataCommand[1] = pBlock[NumBytesWritten++];
		DataCommand[2] = pBlock[NumBytesWritten++];

		if (NumBytesWritten <= 2)
			SST25_SPI_WriteBlock(DataCommand + 1, 2);
		else
			SST25_SPI_WriteBlock(DataCommand, 3);
		SST25_End();

		SST25_Start();
		while (!HAL_GPIO_ReadPin(GENSPI_MISO_GPIO_Port, GENSPI_MISO_Pin));
		SST25_End();
	}
	SST25_Start();
	SST25_SPI_ReadWriteByte(FLASH_WRITE_DISABLE);
	SST25_End();
	SST25_Start();
	SST25_SPI_ReadWriteByte(FLASH_DISABLE_SO);
	SST25_End();

	if (NumBytesRemaining)
		SST25_WriteByte(Address + NumBytesWritten, pBlock[NumBytesWritten]);
}

void SST25_ReadBlock(uint32_t Address, uint8_t *pBlock, uint16_t NumBytes)
{
	Address &= 0xFFFFFF;

	SST25_Start();
	uint8_t Command[] =
	{
			FLASH_READ_FAST,
			(Address >> 16) & 0xFF,
			(Address >> 8) & 0xFF,
			Address & 0xFF,
			0xFF // Dummy byte
	};
	SST25_SPI_WriteBlock(Command, 5);
	SST25_SPI_ReadBlock(pBlock, NumBytes);
	SST25_End();
}

static inline void SST25_Start()
{
  SST25_SPI_Begin();

  SST25_Delay();
}

static inline void SST25_End()
{
  SST25_SPI_End();

  SST25_Delay();
}

static inline void SST25_Delay()
// Warning: Requires CPU_CYCLES to be enabled!
{
  uint32_t End_NumCycles;

  End_NumCycles = CPU_CYCLES + 10; // !!! Hardwired!
  do {} while (CPU_CYCLES < End_NumCycles);
}
