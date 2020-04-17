#ifndef __RC522_H
#define __RC522_H

#include "bsp_rfid.h"
#include "rc522_conf.h"

// 操作rc522芯片函数

/////////////////////////////////////////////////////////////////////
//MF522功能函数
/////////////////////////////////////////////////////////////////////
byte RC_Raw_Read(uint8_t ucAddress);
void RC_Raw_Write(uint8_t ucAddress, uint8_t ucValue);
void RC_Raw_SetMask(uint8_t ucReg, uint8_t ucMask);
void RC_Raw_ClearMask(uint8_t ucReg, uint8_t ucMask);
void RC_Reset(void);
void RC_Antenna_On(void);
void RC_Antenna_Off(void);

#endif
