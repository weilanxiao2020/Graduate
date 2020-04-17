#include  "utils.h"
#include <stdlib.h>
#include <string.h>

uint32_t u8_to_u32(uint8_t* data)
{
	uint32_t res = 0;
	res = res|*data;
	res<<=8;
	res = res|*(data+1);
	res<<=8;
	res = res|*(data+2);
	res<<=8;
	res = res|*(data+3);
	return res;
}

uint64_t u8_to_u64(uint8_t *data)
{
	uint64_t res = 0;
	res = res|*data;
	res<<=8;
	res = res|*(data+1);
	res<<=8;
	res = res|*(data+2);
	res<<=8;
	res = res|*(data+3);
	res<<=8;
	res = res|*(data+4);
	res<<=8;
	res = res|*(data+5);
	res<<=8;
	res = res|*(data+6);
	res<<=8;
	res = res|*(data+7);
	return res;
}

void u32_to_u8(uint32_t data, uint8_t* res)
{
	*(res+3) = data & 0xff;
	data>>=8;
	*(res+2) = data & 0xff;
	data>>=8;
	*(res+1) = data & 0xff;
	data>>=8;
	*(res+0) = data & 0xff;
}

void u64_to_u8(uint64_t data, uint8_t* res)
{
	*(res+7) = data & 0xff;
	data>>=8;
	*(res+6) = data & 0xff;
	data>>=8;
	*(res+5) = data & 0xff;
	data>>=8;
	*(res+4) = data & 0xff;
	data>>=8;
	*(res+3) = data & 0xff;
	data>>=8;
	*(res+2) = data & 0xff;
	data>>=8;
	*(res+1) = data & 0xff;
	data>>=8;
	*(res+0) = data & 0xff;
}

uint32_t str_to_u32(uint8_t *str)
{
	uint32_t result = 0;
	return result;
}
uint8_t* u32_to_str(uint32_t data)
{
	// 2^32 = 0~4,294,967,2965
	uint8_t temp[10];
    // 字符串长度（以包含\0）
	uint8_t len = 0;
    uint8_t i = 0;
    uint32_t num = data;
	uint8_t* res = NULL;
    do{
        temp[i] = num%10;
        num/=10;
        i++;
    }while(num!=0);
    len = i+1;
    res = (uint8_t*)calloc(len, sizeof(uint8_t));
    // printf("数字%lu的长度为%d\n",data, i);
    for(i = 0;i<len-1;i++)
    {
        *(res+i) = temp[len-2-i]+'0';
    }
    *(res+len-1)='\0';
    // printf("数字%lu的str为:%s,address:%p\n",data, res,res);
    return res;
}

void Buf_Clear(byte* data, byte size)
{
	byte i;
	for(i=0;i<size;i++)	*(data+i) = 0x00;
}

boolean Find_Str(const char *src, const char *match)
{ 
	return strstr(src, match) != NULL ? true : false;	
}
