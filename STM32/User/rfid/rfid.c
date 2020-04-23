#include "rfid.h"
#include <string.h>
//#include <malloc.h>
#include <stdlib.h>


void Rfid_Init(void)
{
	Rfid_Gpio_Init();
	Rfid_Spi_Init();	
	RFID_RST_Clr();	
	RFID_CS_Clr();
	Debug_Info(Rfid_TAG, "rfid running...");
}

void Rfid_Card_Set(uint8_t cardType)
{
	sUcType = cardType;
	Debug_Info(Rfid_TAG, "cardType...");
}

uint8_t Rfid_Card_Get(void)
{
 	return sUcType;
}

// 寻卡
uint8_t Rfid_Find_Card(byte* pId)
{
	static uint8_t ucLineCount = 2; 	
	uint8_t flag = Pcd_Request(PICC_REQALL, pId);
	/*寻卡*/
	if (flag != MI_OK)  
		/*若失败再次寻卡*/
		flag = Pcd_Request(PICC_REQALL, pId);	
	                                                	
	if ( flag == MI_OK  )
	{
		/*防冲撞（当有多张卡进入读写器操作范围时，防冲突机制会从其中选择一张进行操作）*/
		if (Pcd_Anticoll(pId) == MI_OK )                                                                   
		{
			// Led_On();
			ucLineCount++;
			if ( ucLineCount == 10 ) ucLineCount = 2;				
		}		
	}		
	return flag;
}

void printf_hex(const char *str, const byte *data, uint8_t len) 
{
	uint8_t i;
	printf("%s data hex=0x", str);
	for(i=0;i<len;i++)
	{
		printf("%02x", *(data+i));
	}
	printf("\n");
	delay_ms(100);
}

// 读取卡片
RfidType *Rfid_Read(uint8_t cardType)
{                                                                                                                                                                                
	uint8_t pId[4];/*先后存放IC卡的类型和UID(IC卡序列号)*/   
	CardMap maps[5] = {TrackId, Timestamp, Sender, Receiver, Address};
	byte data[16]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
	RfidType* rfid = (RfidType *)calloc(1, sizeof(RfidType));	
	Model *model = (Model *)calloc(1, sizeof(Model));
	rfid->cardId = DEFAULT_CARD_ID;
	rfid->cardType = cardType;
	printf_hex("Rfid_Read buf", data, 16);
	if(Rfid_WR_Data(cardType, data, Timestamp, Read))
	{
		printf_hex("read sender", data, 16);
		byte_16_convert_byte_n(data, pId, 4);
		model->sender = u8_to_u32(pId);
		Debug_Info(Rfid_TAG, "read sender 0x%x", model->sender);
		rfid->pData = model;
		Led_Glint(1);
	}
	return rfid;
}

RfidType *Rfid_Read_All(uint8_t cardType) 
{ 
	CardMap maps[6] = {TrackId, Timestamp, Sender, Receiver, Address, Status};
	byte *data = (byte *)calloc(5,sizeof(byte)*16);
	RfidType* rfid = (RfidType *)calloc(1, sizeof(RfidType));	
	Model *model = (Model *)calloc(1, sizeof(Model));
	rfid->cardId = DEFAULT_CARD_ID;
	rfid->cardType = cardType;
	if(Rfid_WR_Datas(cardType, data, maps, 6, Read))
	{
		char trackId[16];
		char address[16];
		uint8_t status;
		// printf_hex("trackId buf read", data, 16);
		// printf_hex("timestamp buf read", data+16, 16);
		// printf_hex("sender buf read", data+32, 16);
		// printf_hex("receiver buf read", data+48, 16);
		// printf_hex("address buf read", data+64, 16);
		// printf_hex("status buf read", data+80, 16);

		byte_16_convert_byte_n(data, model->trackId, 16);
		byte_16_convert_byte_n(data+16, data+16, 8);
		byte_16_convert_byte_n(data+32, data+32, 4);
		byte_16_convert_byte_n(data+48, data+48, 4);
		byte_16_convert_byte_n(data+64, model->address, 16);
		byte_16_convert_byte_n(data+80, &status, 1);

		// model->trackId = trackId;
		model->timestamp = u8_to_u64(data+16);
		model->sender = u8_to_u32(data+32);
		model->receiver = u8_to_u32(data+48);
		// model->address = address;
		model->status = status;
		rfid->pData = model;
		Led_Glint(1);
	}
	free(data);
	return rfid;
}

RfidType *Rfid_Read_TrackId_Address_Status(uint8_t cardType)
{
	CardMap maps[3] = {TrackId, Address, Status};
	byte *data = (byte *)calloc(3,sizeof(byte)*16);
	RfidType *rfid = (RfidType *)calloc(1, sizeof(RfidType));	
	Model *model = (Model *)calloc(1, sizeof(Model));
	rfid->cardId = DEFAULT_CARD_ID;
	rfid->cardType = cardType;
	if(Rfid_WR_Datas(cardType, data, maps, 3, Read))
	{
		byte_16_convert_byte_n(data, model->trackId, 16);
		byte_16_convert_byte_n(data+16, model->address, 16);
		byte_16_convert_byte_n(data+32, &(model->status), 1);
		rfid->pData = model;
		Led_Glint(1);
	}
	free(data);
	return rfid;
}

void Rfid_Write_TrackId_Address_Status(const RfidType* pRfid)
{
	CardMap maps[3] = {TrackId, Address, Status};
	byte *data = (byte *)calloc(3, sizeof(byte)*16);
	char *trackId = pRfid->pData->trackId;
	char *address = pRfid->pData->address;
	uint8_t status = pRfid->pData->status;
	byte_n_convert_byte_16(trackId, 16, data);
	byte_n_convert_byte_16(address, 16, data+16);
	byte_n_convert_byte_16(&status, 1, data+32);
	Rfid_WR_Datas(pRfid->cardType, data, maps, 3, Write);
	free(data);
}

void Rfid_Write_All(const RfidType* pRfid)
{
	CardMap maps[6] = {TrackId, Timestamp, Sender, Receiver, Address, Status};
	byte *data = (byte *)calloc(6, sizeof(byte)*16);
	char *trackId = pRfid->pData->trackId;
	char str[16];
	// uint64_t timestamp = pRfid->pData->timestamp;
	// uint32_t sender = pRfid->pData->sender;
	// uint32_t receiver = pRfid->pData->receiver;
	char *address = pRfid->pData->address;
	uint8_t status = pRfid->pData->status;
	Debug_Info(Rfid_TAG, trackId);
	Debug_Info(Rfid_TAG, address);
	// u32_to_u8(trackId, data);
	// u64_to_u8(timestamp, data+16);
	// u32_to_u8(sender, data+32);
	// u32_to_u8(receiver, data+48);
	// u32_to_u8(address, data+64);
	// byte_n_convert_byte_16(data, 4, data);
	// byte_n_convert_byte_16(data+16, 8, data+16);
	// byte_n_convert_byte_16(data+32, 4, data+32);
	// byte_n_convert_byte_16(data+48, 4, data+48);
	// byte_n_convert_byte_16(data+64, 4, data+64);
	// u32_to_u8(trackId, data);
	// u64_to_u8(timestamp, data+16);
	// u32_to_u8(sender, data+32);
	// u32_to_u8(receiver, data+48);
	// u32_to_u8(address, data+64);
	byte_n_convert_byte_16(trackId, 16, data);
	byte_n_convert_byte_16(data+16, 8, data+16);
	byte_n_convert_byte_16(data+32, 4, data+32);
	byte_n_convert_byte_16(data+48, 4, data+48);
	byte_n_convert_byte_16(address, 16, data+64);
	byte_n_convert_byte_16(&status, 1, data+80);

	Rfid_WR_Datas(pRfid->cardType, data, maps, 6, Write);
	free(data);
}

void Rfid_Write_Sender(const RfidType* pRfid)
{
	byte sender[16];
	Debug_Info(Rfid_TAG, "Rfid_Write_Sender");
	u32_to_u8(pRfid->pData->sender, sender);
	byte_n_convert_byte_16(sender, 4, sender);
	printf_hex("Rfid_Write_Sender buf", sender, 16);
	if(Rfid_WR_Data(pRfid->cardType, sender, Sender, Write))
	{
		Debug_Info(Rfid_TAG, "write sender 0x%x", pRfid->pData->sender);
		Led_Glint(1);
	}
}

// 读写数据
/**
 * @param cardType 	卡片类型
 * @param pData		数据指针
 * @param map		区域映射
 * @param mode		操作类型
 */
boolean Rfid_WR_Data(uint8_t cardType, byte *pData, byte map, RfidMode mode)
{
	uint8_t pId[4];/*先后存放IC卡的类型和UID(IC卡序列号)*/  
	char status;
	char cnt;
	uint8_t Default_Key[DEFAULT_KEY_LENGTH];
	uint8_t sector = (map&0xf0)>>4;
	uint8_t block = map&0x0f;
	// Debug_Info(Rfid_TAG, "map 映射区域=0x%02x->第%x扇区,第%d块", map, sector, block);
	for(cnt = 0; cnt < 6; cnt++) Default_Key[cnt]=0xff;
	// 重置
	Pcd_Reset();
	Pcd_Config_ISO_Type(cardType);
	// 一直读卡，直到时间完成
	while ( 1 )
	{ 
		Led_Glint(1);
		if (Rfid_Find_Card(pId) == MI_OK)
		{
			// 选卡	
			status = Pcd_Select(pId);
			if(status == MI_OK)
			{
				// 认证
				// 校验1扇区密码，密码位于每一扇区第3块，验证A密钥，块地址，扇区密码，卡序列号
				status = Pcd_Auth_State(KEYA, (sector*4+3), Default_Key, pId);   
				{
					// Debug_Info(Rfid_TAG, "select card id:0x%x", u8_to_u32(pId));
					if(status == MI_OK)
					{
						if(mode == Read) status = Pcd_Read((sector*4+block), pData); 
						else if(mode == Write) status = Pcd_Write((sector*4+block), pData); 
						// status = PcdWrite((sector*4+block), data);   // 写卡，将buf[0]-buf[16]写入1扇区0块
						if(status == MI_OK) 
						{
							//读写成功，串口提示信息，oled提示
							// Tips_Success();
							Debug_Info(Rfid_TAG, "读写数据成功");
							delay_ms(1000);
							// Pcd_Wait_Card_Off();
							Debug_Info(Rfid_TAG, "等待卡片离开");
							return true;
						}
					}
				}
			}
		}
	}	
	return false;
}

// 读取多块数据
/**
 * @param cardType 	卡片类型
 * @param pDatas	数据指针 byte *data16字节
 * @param maps		区域映射
 * @param length	区域映射数
 * @param mode		操作类型
 */
boolean Rfid_WR_Datas(uint8_t cardType, byte *pDatas, const CardMap *maps, uint8_t length, RfidMode mode)
{
	uint8_t pId[4];/*先后存放IC卡的类型和UID(IC卡序列号)*/  
	char status;
	char cnt,i;
	uint8_t flag = 0x00;
	uint8_t isComplete = false;
	uint8_t sector;
	uint8_t block;
	uint8_t Default_Key[DEFAULT_KEY_LENGTH] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	// 重置
	Pcd_Reset();
	Pcd_Config_ISO_Type(sUcType);
	// 一直读卡，直到时间完成
	Debug_Info(Rfid_TAG, "读写数据...");
	while (1)
	{ 
		if (Rfid_Find_Card(pId) == MI_OK)
		{
			// 选卡	
			status = Pcd_Select(pId);
			if(status == MI_OK)
			{
				// 认证
				// 校验1扇区密码，密码位于每一扇区第3块，验证A密钥，块地址，扇区密码，卡序列号
				int i;
				for(i=0;i<length;i++)
				{
					sector = (*(maps+i)&0xf0)>>4;
					block = *(maps+i)&0x0f;
					// Debug_Info(Rfid_TAG, "map 映射区域=0x%02x->第%x扇区,第%d块", *(maps+i),  sector, block);
					// 重置密码
					for(cnt = 0; cnt < 6; cnt++) Default_Key[cnt]=0xff;
					status = Pcd_Auth_State(KEYA, (sector*4+3), Default_Key, pId);   
					{
						// Debug_Info(Rfid_TAG, "select card id:0x%x", u8_to_u32(pId));
						if(status == MI_OK)
						{
							if(mode == Read) status = Pcd_Read((sector*4+block), pDatas+16*i); 
							else if(mode == Write) status = Pcd_Write((sector*4+block), pDatas+16*i); 
							if(status == MI_OK) 
							{
								// Debug_Info(Rfid_TAG, "读写数据成功");
								// printf("map 映射区域=0x%x\n", *(maps+i));
								flag++;
								//读写成功，串口提示信息，oled提示
							}
						}
					}
				}
				if(flag==length) 
				{
					Debug_Info(Rfid_TAG, "读写所有数据成功");
					// delay_ms(1000);
					Pcd_Wait_Card_Off();
					Debug_Info(Rfid_TAG, "等待卡片离开");
					return true;
				}
			}
		}
	}
	return false;
}


/**
 * 将16字节数组从地位拷贝到n字节数组
 * @param src		16字节数组
 * @param res		n字节数组
 * @param destLen	n字节数组长度
 */
boolean byte_16_convert_byte_n(const byte *src, byte *dest, uint8_t destLen)
{
	uint8_t i;
	uint8_t cnt = 16-destLen;
	if(cnt<0) return false;
	for(i=0;i<destLen;i++)
	{
		*(dest+i) = *(src+cnt+i);
	}
	return true;
}

/**
 * 将n字节数组从地位拷贝到16字节数组
 * @param src		n字节数组
 * @param resLen	n字节数组长度
 * @param res		16字节数组
 */
boolean byte_n_convert_byte_16(const byte *src, uint8_t srcLen, byte *dest) 
{
	int8_t i;
	uint8_t cnt = 16-srcLen;
	if(cnt<0) return false; 
	for(i=srcLen-1;i>-1;i--)
	{
		*(dest+cnt+i) = *(src+i);
	}
	for(i=cnt-1;i>-1;i--)
	{
		*(dest+i) = 0x00;
	}

	return true;
}