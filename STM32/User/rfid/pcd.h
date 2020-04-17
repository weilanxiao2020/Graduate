#ifndef __PCD_H
#define __PCD_H

#include "rc522.h"
#include "common.h"
#include "constants.h"

void Pcd_Config_ISO_Type(uint8_t ucType);
void Pcd_Wait_Card_Off(void);
char Pcd_Com_MF522(uint8_t ucCommand, uint8_t* pInData, uint8_t ucInLenByte, uint8_t* pOutData, uint32_t* pOutLenBit );
char Pcd_Request(uint8_t ucReq_code, uint8_t* pTagType);
char Pcd_Anticoll(uint8_t* pSnr);
void Calulate_CRC(uint8_t* pIndata, uint8_t ucLen, uint8_t* pOutData);
char Pcd_Select(uint8_t* pSnr);
char Pcd_Auth_State(uint8_t ucAuth_mode, uint8_t ucAddr, uint8_t* pKey, uint8_t* pSnr );
char Pcd_Write(uint8_t ucAddr, uint8_t* pData);
char Pcd_Read(uint8_t ucAddr, uint8_t* pData);
char Pcd_Halt(void);
void IC_CMT(uint8_t* UID, uint8_t* KEY, uint8_t RW, uint8_t* Dat );
void Pcd_Reset(void);


#endif
