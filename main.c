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
/* defines -------------------------------------------------------------------*/
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
		  switch (Key_Map)
		  {
			  case 0x01:break;
			  case 0x02:break;
			  case 0x04:break;
			  case 0x08:break;
		  }
	  }
	  else 
	  {
		 Key_Map_bk=0; 
	  }
}

char Print_Buf[256];
void uart1_printf(char *fmt,...)
{
	unsigned char len;
    va_list ap;
	
    va_start(ap,fmt);
    len = vsprintf(Print_Buf,fmt,ap);
	UARTTransmitLin(UART1,Print_Buf,len);
	 
    va_end(ap);
}


void uartReceive(void)
{
	if((CSP_UART_GET_ISR(UART0)))
	{
		CSP_UART_SET_ISR(UART1,UART_RX_INT_S);
		RxDataFlag = TRUE;
		char c = UART_ReturnRxByte(UART0);
		UARTTxByte(UART0,(U8_T*) c);
	}
}

#define RLY1_OUT; GPIO_Init(GPIOB0,3,1);
#define RLY2_OUT; GPIO_Init(GPIOB0,2,1);
#define PWM1_OUT; GPIO_Init(GPIOA0,5,1);
#define PWM2_OUT; GPIO_Init(GPIOC0,0,1);

#define RLY1_PullHigh;	GPIO_PullHigh_Init(GPIOB0,3);
#define RLY2_PullHigh;	GPIO_PullHigh_Init(GPIOB0,2);
#define PWM1_PullHigh;	GPIO_PullHigh_Init(GPIOA0,5);
#define PWM2_PullHigh;	GPIO_PullHigh_Init(GPIOC0,0);

#define RLY1_HIGH; 		GPIO_Set_Value(GPIOB0,3,1);
#define RLY2_HIGH; 		GPIO_Set_Value(GPIOB0,2,1);
#define PWM1_HIGH; 		GPIO_Set_Value(GPIOA0,5,1);
#define PWM2_HIGH; 		GPIO_Set_Value(GPIOC0,0,1);



void GPIO_PWM_RLY_Init(void)
{
	
	RLY1_OUT; 
	RLY2_OUT;
	PWM1_OUT;
	PWM2_OUT;
	
	RLY1_PullHigh;
	RLY2_PullHigh;
	PWM1_PullHigh;
	PWM2_PullHigh;
	
	RLY1_HIGH;
	RLY2_HIGH;
	PWM1_HIGH;
	PWM2_HIGH;
}

/*************************************************************/
//main
/*************************************************************/
int timer;
int main(void)
{
	APT32F101_init();
	UART_CONFIG();
	GPIO_PWM_RLY_Init();
	
    while(1)
	{
//		TK_PRGM();
		timer++;
		if(timer%100000==0)
		uart1_printf("%s\r\n","$MCU");
    }
}
/******************* (C) COPYRIGHT 2016 APT Chip *****END OF FILE****/


