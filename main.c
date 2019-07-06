/*
  ******************************************************************************
  * @file    main.c
  * @author  APT AE Team
  * @version V1.24
  * @date    2018/10/15
  ******************************************************************************
  *THIS SOFTWARE WHICH IS FOR ILLUSTRATIVE PURPOSES ONLY WHICH PROVIDES 
  *CUSTOMER WITH CODING INFORMATION REGARDING THEIR PRODUCTS.
  *APT CHIP SHALL NOT BE HELD RESPONSIBILITY ADN LIABILITY FOR ANY DIRECT, 
  *INDIRECT DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF 
  *SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION 
  *CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.AND APT CHIP RESERVES 
  *THE RIGHT TO MAKE CHANGES IN THE SOFTWARE WITHOUT NOTIFICATION
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "apt32f101.h"
#include "apt32f101_gpio.h"
#include "apt32f101_uart.h"
#include "apt32f101_counta.h"
#include "apt32f101_i2c.h"
#include "apt32f101_led.h"
#include "apt32f101_gtc.h"
#include "apt32f101_ifc.h"
#include "apt32f101_adc.h"
#include "apt32f101_syscon.h"
#include "apt32f101_tkey.h"

#include <stdarg.h>


#define  clk_H; GPIO_Write_High(GPIOB0,1);
#define  dio_H; GPIO_Write_High(GPIOB0,0);
#define  stb_H; GPIO_Write_High(GPIOA0,0);

#define  clk_L; GPIO_Write_Low(GPIOB0,1);
#define  dio_L; GPIO_Write_Low(GPIOB0,0);
#define  stb_L; GPIO_Write_Low(GPIOA0,0);


U16_T Key_Map_bk=0;
/* externs--------------------------------------------------------------------*/
extern void APT32F101_init(void);
extern S32_T Sampling_Data[16];									//sampling data
extern S32_T Baseline_Data[16];									//baseline data
extern S32_T Offset_Data[16];									//offset data 
extern U16_T Key_Map;											//touch key map
extern volatile uint8_t I2CWrBuffer[BUFSIZE];
extern volatile uint8_t I2CRdBuffer[BUFSIZE];
/* Functions-------------------------------------------------------------------*/
void TK_PRGM(void)
{
	  //Read_Sampling();				//read sampling data for testing
	  //Read_Baseline();				//read daseline data for testing
	  //Read_Offset();					//read offset for testing,offset=daseline-sampling	
	  Read_Keymap();					//read key data,if Key_Map!=0, touch key triggered
	  if (Key_Map!=0)
	  {
		  if (Key_Map==Key_Map_bk)return;
		  Key_Map_bk=Key_Map;
		  uart1_printf("Key_Map:%d\r\n",Key_Map);
		  
	  }
	  else 
	  {
		 Key_Map_bk=0; 
	  }
}


char Print_Buf[64];
void uart1_printf(char *fmt,...)
{
	unsigned char len;
    va_list ap;
	
    va_start(ap,fmt);
    len = vsprintf(Print_Buf,fmt,ap);
	UARTTransmitLin(UART1,Print_Buf,len);
	 
    va_end(ap);
}

void tm1616write(unsigned char wr_date)
{
	unsigned char i;
	stb_L;
	nop;
	for(i = 0; i < 8; i++)
	{
		clk_L;
		nop;
		if(wr_date&0x01 != 0)
		{
			dio_H;
		}
		else
		{
			dio_L;
		}
		nop;
		clk_H;
		nop;
		wr_date = wr_date >> 1;
	}	
}

void tm1616show1(unsigned char data)
{
	unsigned char j;
	clk_H; 
	dio_H;
	stb_H; 
	
	
//	tm1616write(0x02);  			//显示模式
//	stb_H;							
	tm1616write(0x44);  			//数据命令设置
	stb_H;	


	tm1616write(0xC0);			//设置显示地址1  00
	tm1616write(data); 		
	stb_H;			
	
	tm1616write(0x8f);				//控制命令设置，设置显示开、显示脉冲宽度4/16  1000 1010
	stb_H;
}

void tm1616show2(unsigned char data)
{
	unsigned char j;
	clk_H; 
	dio_H;
	stb_H; 
	
	tm1616write(0x00);  			//显示模式
	stb_H;							
	tm1616write(0x44);  			//数据命令设置
	stb_H;	


	tm1616write(0xC2);			//设置显示地址1  00
	tm1616write(data); 		
	stb_H;	


	
	
	tm1616write(0x8f);				//控制命令设置，设置显示开、显示脉冲宽度4/16  1000 1010
	stb_H;
}

int num;
//char arr1[8]={0x7e,0x7d,0x7b,0x77,0x6f,0x5f,0x3f,0x7f};
//char arr2[8]={0x00,0x01,0x02,0x04,0x8,0x16,0x32,0x64};

char arr1[8]={0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7f};
char arr2[8]={0x00,0x01,0x02,0x04,0x8,0x10,0x20,0x40};

int main(void)
{
	
	
	APT32F101_init();
	
	GPIO_Init(GPIOB0,0,0); 
	GPIO_Init(GPIOB0,1,0); 
	GPIO_Init(GPIOA0,0,0); 
	
	char i = 0;
	int j = 0;
    while(1)
	{
//		num++;
//		if(num%10000==0){
//			GPIO_Reverse(GPIOA0,5);
//			GPIO_Reverse(GPIOB0,2);
//			GPIO_Reverse(GPIOB0,3);
//			GPIO_Reverse(GPIOC0,0);
//			
//
//		}
		
		for(i=0;i<8;i++){	
			delay_nms(5000);
			tm1616show1(arr1[i]);
			tm1616show2(arr2[i]);
		}
	}
}



