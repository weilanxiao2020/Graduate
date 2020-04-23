//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PA5（SCL）
//              D1   接PA7（SDA）
//              RES  接PB0
//              DC   接PB1
//              CS   接PA4               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
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
//开启OLED显示    
void Oled_Display_On(void)
{
	Oled_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	Oled_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	Oled_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void Oled_Display_Off(void)
{
	Oled_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	Oled_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	Oled_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void Oled_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		Oled_WR_Byte(0xb0+i, OLED_CMD);    //设置页地址（0~7）
		Oled_WR_Byte(0x00, OLED_CMD);      //设置显示位置—列低地址
		Oled_WR_Byte(0x10, OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++) Oled_WR_Byte(0, OLED_DATA); 
	} //更新显示
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void Oled_Show_Char(uint8_t x, uint8_t y, uint8_t chr)
{      	
	uint8_t c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
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
//m^n函数
uint32_t Oled_pow(uint8_t m, uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
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
//显示一个字符号串
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
//显示汉字
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
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
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


//初始化SSD1306					    
void Oled_Init(void)
{ 	
 
 Oled_Gpio_Init();
 	 
// 	GPIO_InitTypeDef  GPIO_InitStructure;
// 	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能A端口时钟
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;	 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOD3,6
// 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_4);	
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能A端口时钟
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8;	 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6
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
	Oled_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	Oled_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
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





























