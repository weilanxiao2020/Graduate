#ifndef __EEPROM_H
#define __EEPROM_H

#include "bsp_iic.h"

#ifdef GZ_DEBUG
	#define E2prom_TAG 			"eeprom"
#else 
	#define E2prom_TAG			""
#endif

#define EEPROM_I2C_Device_Address		0xa0
// #define EEPROM_I2C_Write_Address		0xa0

void E2prom_Init(void);
uint8_t E2prom_Read_Addr();

void E2prom_Write_Byte(uint8_t deviceAddr, uint8_t addr, uint8_t data);
uint8_t E2prom_Read_Byte(uint8_t deviceAddr, uint8_t addr);
void E2prom_Read_Bytes(uint8_t deviceAddr, uint8_t addr, uint8_t *pData, uint8_t len);

#endif
