#ifndef __RFID_H
#define __RFID_H

#include "pcd.h"
#include "bsp_led.h"
#include "utils.h"
#include "debug.h"
#include "constants.h"
#include "common.h"

#ifdef GZ_DEBUG
    #define Rfid_TAG    "rfid"
#else
    #define Rfid_TAG    ""
#endif
// 宏定义
#define DEFAULT_UCTYPE 	'A'
#define DEFAULT_CARD_ID 0x00000000
#define DEFAULT_KEY_LENGTH	6

// 相关结构体定义
typedef struct {
	char trackId[16];		// 快递id
	int64_t	 timestamp;		// 时间戳
	uint32_t sender;		// 发送者
	uint32_t receiver;		// 接收者
	char address[15+1];		// 目的地
	uint8_t status;			// 状态
	// gps信息
}Model;
#define Model_Size			49		

const static uint8_t Model_TrackId_Len = 15;
const static uint8_t Model_Address_Len = 15;

typedef struct {
	uint32_t 	cardId;		// 卡片类型
	uint8_t 	cardType;	// RfidCard数据域，用户自定义
	Model 		*pData;		// 用户数据
}RfidType;

typedef enum {
	Read=0x00,
	Write=0xff
}RfidMode;

typedef enum {
	TrackId = 0x20,
	Timestamp = 0x30,
	Sender = 0x40,
	Receiver = 0x50,
	Address = 0x60,
	Status = 0x70,
}CardMap;

// 用户数据区域
// ISO14443A卡存储区有1kBytes。
// 分为16个扇区，每个扇区有4个块，每块16个字节。
// 其中第1扇区第0块是卡序列号，是只读的，不能写。
// 密钥存放在每个扇区的块3。
// 一个扇区64字节
// 1k Byte = 16(bit) * 4(k块) * 16(扇区)
// #define Read					0x00
// #define Write					0xff
// 订单号
#define TrackId_Sector			1
#define TrackId_Block			0
// 时间戳
#define Timestamp_Sector		2
#define Timestamp_Block			0
// 发送者
#define Sender_Sector			3
#define Sender_Block			0
// 接收者
#define Receiver_Sector			4
#define Receiver_Block			0
// 目的地
#define Address_Sector			5
#define Address_Block			0
// 寻卡类型
static uint8_t sUcType = DEFAULT_UCTYPE;

/////////////////////////////////////////////////////////////////////
void Rfid_Init(void);
RfidType *Rfid_Read(uint8_t cardType);
uint8_t Rfid_Card_Get(void);
void Rfid_Card_Set(uint8_t ucType);
uint8_t Rfid_Find_Card(uint8_t* pId);
boolean Rfid_WR_Data(uint8_t cardType, byte *data, byte map, RfidMode mode);
// 读写多块数据
boolean Rfid_WR_Datas(uint8_t cardType, byte *pDatas, const CardMap *maps, uint8_t length, RfidMode mode);


/*--------------------------------------------------*/
RfidType *Rfid_Read_All(uint8_t cardType);
void Rfid_Write_All(const RfidType* pRfid);
RfidType *Rfid_Read_TrackId_Address_Status(uint8_t cardType);
void Rfid_Write_TrackId_Address_Status(const RfidType* pRfid);

void Rfid_Write_Sender(const RfidType* pRfid);
/*--------------------------------------------------*/
// 16字节转换成n字节
boolean byte_16_convert_byte_n(const byte *src, byte *res, uint8_t resLen);
// n字节转换成16字节
boolean byte_n_convert_byte_16(const byte *src, uint8_t srcLen, byte *res);
#endif
