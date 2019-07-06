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

#define ROW_ONE  0x01
#define ROW_TWO  0x02

int num;
char light[8]={0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7f};
/* externs--------------------------------------------------------------------*/
extern void APT32F101_init(void);
/* Functions-------------------------------------------------------------------*/

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

void tm1616write(U8_T wr_date)
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

void tm1616show(U8_T light_data,U8_T row)
{
	unsigned char j;
	clk_H; 
	dio_H;
	stb_H; 
	
	tm1616write(0x00);  			//显示模式
	stb_H;							
	tm1616write(0x44);  			//数据命令设置
	stb_H;	

	if(row==ROW_ONE){
		tm1616write(0xC0);			//设置显示地址1  00
		tm1616write(light_data); 		
		stb_H;		
	}else if(row==ROW_TWO){
		tm1616write(0xC2);			//设置显示地址1  00
		tm1616write(light_data); 		
		stb_H;	
	}else{}
	
	tm1616write(0x8f);				//控制命令设置，设置显示开、显示脉冲宽度4/16  1000 1010
	stb_H;
}

void PC_TO_MCU(void){
	char c = 0;
	char a = UARTRxByte(UART1,&c);
	
	if(a==TRUE){
		a=FALSE;
		uart1_printf("%c",c);
		
		switch(c)
		{
			case '0':
				tm1616show(light[0],ROW_ONE);
				tm1616show(light[0],ROW_TWO);
			break;
			
			case '1':
				tm1616show(light[1],ROW_ONE);
			break;
			
			case '2':
				tm1616show(light[2],ROW_ONE);
			break;
			
			case '3':
				tm1616show(light[3],ROW_ONE);
			break;
			
			case '4':
				tm1616show(light[4],ROW_ONE);
			break;
			
			case '5':
				tm1616show(light[5],ROW_ONE);
			break;
			
			case '6':
				tm1616show(light[6],ROW_ONE);
			break;
			
			case '7':
				tm1616show(light[7],ROW_ONE);
			break;
			
			case '8':
				GPIO_Reverse(GPIOA0,5);
			break;
			
			
			default:
			break;
		}
		
	}
	
}

void EXTI_PC01_INIT(void){
	GPIO_Init(GPIOC0,1,0); 
	GPIO_PullLow_Init(GPIOC0,0);
	
	GPIO_IntGroup_Set(PC0);
	GPIOC0_EXI_Init(EXI1);
	EXTI_trigger_CMD(ENABLE,EXI_PIN1,_EXIFT);
	EXTI_trigger_CMD(ENABLE,EXI_PIN1,_EXIRT);
	EXTI_interrupt_CMD(ENABLE,EXI_PIN1);
	GPIO_EXTI_interrupt(GPIOC0,0b00000000000010); 
	EXI1_Int_Enable(); 
	EXI1_WakeUp_Enable(); 
}

int main(void)
{
	APT32F101_init();
	
	GPIO_Init(GPIOB0,0,0); 
	GPIO_Init(GPIOB0,1,0); 
	GPIO_Init(GPIOA0,0,0);
	GPIO_Init(GPIOA0,5,0);
 

	
	
    while(1)
	{
		PC_TO_MCU();
	}
}



