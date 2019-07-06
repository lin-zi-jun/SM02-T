/*
  ******************************************************************************
  * @file    apt32f101_tkey_Parameter.c
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

#include "apt32f101_tkey.h"

#define TRIGGLEVEL 0X30
void TKey_Parameter_Init(void)
{
/*************************************************************/
//Touch Key mode Parameters
/*************************************************************/
	TK_mode					=	TK_EX_Mode;						//TK_EX_Mode/TK_EX_Led_Mode
	TK_High_Anti			=	TK_High_Anti_EN;				//TK_High_Anti_EN/TK_High_Anti_DIS(more electrical current in this mode(more 30uA in deepsleep)）
/*************************************************************/
//Touch Key basic Parameters
/*************************************************************/  
//	TK_IO_ENABLE     		=	0B0001111001100110;				//IO Enable,bit15←bit0=TCH15~TCH0    使能触摸通道控制   --->
	TK_IO_ENABLE     		=	0B0001111001100110;
	TK_GSR		     		=	0x02;							//Touch key sensetivity			 全局灵敏度，此值越大灵敏度越高，此值丌建议设置大于 6
	TK_Trigglevel_data[0]	=	TRIGGLEVEL;							//CH0 TriggerlevelTouch key 阈值，此值通常设置为 offset 值的 70%，最大值为255
	TK_Trigglevel_data[1]	=	TRIGGLEVEL;							//CH1 Triggerlevel
	TK_Trigglevel_data[2]	=	TRIGGLEVEL;							//CH2 Triggerlevel
	TK_Trigglevel_data[3]	=	TRIGGLEVEL;							//CH3 Triggerlevel
	TK_Trigglevel_data[4]	=	TRIGGLEVEL;							//CH4 Triggerlevel
	TK_Trigglevel_data[5]	=	TRIGGLEVEL;							//CH5 Triggerlevel
	TK_Trigglevel_data[6]	=	TRIGGLEVEL;							//CH6 Triggerlevel
	TK_Trigglevel_data[7]	=	TRIGGLEVEL;							//CH7 Triggerlevel
	TK_Trigglevel_data[8]	=	TRIGGLEVEL;							//CH8 Triggerlevel
	TK_Trigglevel_data[9]	=	TRIGGLEVEL;							//CH9 Triggerlevel
	TK_Trigglevel_data[10]	=	TRIGGLEVEL;							//CH10 Triggerlevel
	TK_Trigglevel_data[11]	=	TRIGGLEVEL;							//CH11 Triggerlevel
	TK_Trigglevel_data[12]	=	TRIGGLEVEL;							//CH12 Triggerlevel
	TK_Trigglevel_data[13]	=	TRIGGLEVEL;							//CH13 Triggerlevel
	TK_Trigglevel_data[14]	=	TRIGGLEVEL;							//CH14 Triggerlevel
	TK_Trigglevel_data[15]	=	TRIGGLEVEL;							//CH15 Triggerlevel
	Longpress_ctrl			=	TK_longpress_Enable;			//TK_longpress_Enable/TK_longpress_Disable   使能时将开始长按检测功能，达到检测时间强制复位触摸按键
	Longpress_time	 		=	_16s;							//longpress_time = _16s/_32s/_48s/_64s    长按检测功能使能时有效，默认 16s
//	Key_mode				=	FirstKey;						//TK mode FirstKey/Multikey		按键模式分为 FirstKey 和 Multikey，使用组合键时可设置为 Multikey
	Key_mode				=	Multikey;
	Key_Debounce			=	KEYDETDEB2;						//KEYDETDEB0~3	按键内部去抖时间。建议客户使用时，同时增加软件去抖
/*************************************************************/
//Touch Key Special Control Parameters
/*************************************************************/ 
	TKPDIV					=	TKPDIV2;						//TK Scan clock divider TKPDIV0~5    扫描时钟分频
	TKHYST					=	TKHYST0;						//TK release detection point TKHYST0~1   释放探测点？
//HW0 works only at the condition that sampled data is not less than baseline value  只在采样数据不小于基线值的情况下工作
	HW0_BLUPD_THR			=	BLUPD_THR0;						//Baseline value update threshold BLUPD_THR0~3   基线值阈值
	HW0_BLUPD_WCO0			=	BLUPD_WCO0_1;					//Baseline update weight coefficient when sampling<BLUPD_THR BLUPD_WCO0_0~3 采样时基线更新权重系数
	HW0_BLUPD_WCO1			=	BLUPD_WCO1_1;					//Baseline update weight coefficient when sampling>BLUPD_THR BLUPD_WCO0_0~3
	HW0_BLUPD_DEB0			=	0X05;							//Baseline update debounce when sampling<BLUPD_THR 0x00~0xf					基线更新在抽样时失效
	HW0_BLUPD_DEB1			=	0X32;							//Baseline update debounce when sampling>BLUPD_THR 0x00~0xff
	HW0_ABNFLT				=	ABNFLT0;						//Abnormal data filter ABNFLT0~3			异常数据过滤
	HW0_BLUPD_THVAL			=	0x0A;							//Automatic baseline update threshold 0x00~0xff   自动基线更新阈值
//HW1 works only at the condition that sampled data is less than baseline value  只在采样数据小于基线值的情况下工作
	HW1_BLUPD_THR			=	BLUPD_THR0;						//Baseline value update threshold BLUPD_THR0~3   基线值更新阈值
	HW1_BLUPD_WCO0			=	BLUPD_WCO0_1;					//Baseline update weight coefficient when sampling<BLUPD_THR BLUPD_WCO0_0~3
	HW1_BLUPD_WCO1			=	BLUPD_WCO1_3;					//Baseline update weight coefficient when sampling>BLUPD_THR BLUPD_WCO0_0~3
	HW1_BLUPD_DEB0			=	0X05;							//Baseline update debounce when sampling<BLUPD_THR 0x00~0xf
	HW1_BLUPD_DEB1			=	0X32;							//Baseline update debounce when sampling>BLUPD_THR 0x00~0xff
	HW1_ABNFLT				=	ABNFLT0;						//Abnormal data filter in EX Mode ABNFLT0~3
	HW1_BLUPD_THVAL			=	0x0A;							//Automatic baseline update threshold 0x00~0xff
//-------------------------------------------------------
	OFFSET_MUL				=	OFFSET0;						//OFFSET auto-multiply OFFSET0~3
	TOSCFREQ				=	TKFREQ0;						//TKFREQ0/2/25 (RC mode default=TKFREQ25)  RC模式默认
	CMPFLT					=	CMPFLT2;						//digital filter configuration CMPFLT0~3   数字滤波器的配置
	BLUPDICNT				=	BLUPDICNT2;						//Detecting period of automatic baseline update  BLUPDICNT0~3  自动基线更新检测周期
	TOSCFDIV				=	TOSCFDIV0;						//channel sensing oscillator pre-divider in RC mode TOSCFDIV0~15  通道传感振荡器预分频器
	ICNTSWEN_ctrl			=	ICNTSWDIS;						//Interval scan period auto-switch configuration ICNTSWDIS/ICNTSWEN 间隔扫描周期自动切换配置
	BLUPDIS_ctrl			=	BASE_EN;						//Baseline automatic update disable BASE_EN/BASE_DIS  基线自动更新禁用
	BLUPDIS_CYCLE_VAL		=	0x08;							//Disable baseline value updating cycles after release  在发布后禁用基线值更新周期
}


