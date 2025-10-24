// Written by P J Bladen 2018.

#ifndef __PJB_SST25V_H
#define __PJB_SST25V_H

#include "stdint.h"

void SST25_ReadBlock(uint32_t Address, uint8_t *pBlock, uint16_t NumBytes);
void SST25_WriteByte(uint32_t Address, uint8_t Value);
void SST25_WriteBlock(uint32_t Address, uint8_t *pBlock, uint16_t NumBytes);
void SST25_EraseSector_4K(uint32_t Address);

#endif
