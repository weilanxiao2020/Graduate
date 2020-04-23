//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PA5��SCL��
//              D1   ��PA7��SDA��
//              RES  ��PB0
//              DC   ��PB1
//              CS   ��PA4               
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/

#include "oled.h"
#include "common.h"
#include "stdlib.h"
#include "oledfont.h"  	
#include "bsp_oled.h"

void Oled_Set_Pos(uint8_t x, uint8_t y) 
{ 
	Oled_WR_Byte(0xb0+y,OLED_CMD);
	Oled_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	Oled_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//����OLED��ʾ    
void Oled_Display_On(void)
{
	Oled_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	Oled_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	Oled_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void Oled_Display_Off(void)
{
	Oled_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	Oled_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	Oled_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void Oled_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		Oled_WR_Byte(0xb0+i, OLED_CMD);    //����ҳ��ַ��0~7��
		Oled_WR_Byte(0x00, OLED_CMD);      //������ʾλ�á��е͵�ַ
		Oled_WR_Byte(0x10, OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++) Oled_WR_Byte(0, OLED_DATA); 
	} //������ʾ
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void Oled_Show_Char(uint8_t x, uint8_t y, uint8_t chr)
{      	
	uint8_t c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			Oled_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			Oled_WR_Byte(F8X16[c*16+i],OLED_DATA);
			Oled_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			Oled_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				Oled_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				Oled_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n����
uint32_t Oled_pow(uint8_t m, uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void Oled_Show_Num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/Oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				Oled_Show_Char(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	Oled_Show_Char(x+(size/2)*t,y,temp+'0'); 
	}
} 
//��ʾһ���ַ��Ŵ�
void Oled_Show_String(uint8_t x, uint8_t y, uint8_t *chr)
{
	uint8_t j=0;
	while (chr[j]!='\0')
	{		Oled_Show_Char(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//��ʾ����
void Oled_Show_Chinese(uint8_t x, uint8_t y, uint8_t no)
{      			    
	uint8_t t,adder=0;
	Oled_Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
		Oled_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
	}	
	Oled_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
		Oled_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
	}					
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void Oled_Draw_Bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *ptrBmp)
{ 	
 unsigned int j=0;
 uint8_t x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		Oled_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	Oled_WR_Byte(ptrBmp[j++],OLED_DATA);	    	
	    }
	}
} 


//��ʼ��SSD1306					    
void Oled_Init(void)
{ 	
 
 Oled_Gpio_Init();
 	 
// 	GPIO_InitTypeDef  GPIO_InitStructure;
// 	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��A�˿�ʱ��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;	 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
// 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_4);	
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��A�˿�ʱ��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8;	 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
// 	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8);	



 
  OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(200);
	OLED_RST_Set(); 
					  
	Oled_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	Oled_WR_Byte(0x00,OLED_CMD);//---set low column address
	Oled_WR_Byte(0x10,OLED_CMD);//---set high column address
	Oled_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	Oled_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	Oled_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	Oled_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	Oled_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	Oled_WR_Byte(0xA6,OLED_CMD);//--set normal display
	Oled_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	Oled_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	Oled_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	Oled_WR_Byte(0x00,OLED_CMD);//-not offset
	Oled_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	Oled_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	Oled_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	Oled_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	Oled_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	Oled_WR_Byte(0x12,OLED_CMD);
	Oled_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	Oled_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	Oled_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	Oled_WR_Byte(0x02,OLED_CMD);//
	Oled_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	Oled_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	Oled_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	Oled_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	Oled_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	Oled_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	Oled_Clear();
	Oled_Set_Pos(0,0); 	
}  





























