/******************************************************
**  工程名称：UNV-01语音识别模块驱动程序
**	CPU: STC11L08XE
**	晶振：22.1184MHZ
**	波特率：9600 bit/S
***************************/
#include "config.h"
/************************************************************************************/
//	nAsrStatus 用来在main主程序中表示程序运行的状态，不是LD3320芯片内部的状态寄存器
//	LD_ASR_NONE:		表示没有在作ASR识别
//	LD_ASR_RUNING：		表示LD3320正在作ASR识别中
//	LD_ASR_FOUNDOK:		表示一次识别流程结束后，有一个识别结果
//	LD_ASR_FOUNDZERO:	表示一次识别流程结束后，没有识别结果
//	LD_ASR_ERROR:		表示一次识别流程中LD3320芯片内部出现不正确的状态
/***********************************************************************************/
uint8 idata nAsrStatus=0;	
void MCU_init(); 
void ProcessInt0(); //识别处理函数
void delay(unsigned long uldata);
void User_handle(uint8 dat);//用户执行操作函数
void Delay200ms();
void Led_test(void);//单片机工作指示
uint8_t G0_flag=DISABLE;//运行标志，ENABLE:运行。DISABLE:禁止运行 
sbit LED=P4^2;//信号指示灯
sbit P10=P1^0;
sbit P11=P1^1;
sbit P12=P1^2;
sbit P13=P1^3;
sbit P14=P1^4;

/***********************************************************
* 名    称： void  main(void)
* 功    能： 主函数	程序入口 
**********************************************************/ 
void  main(void)
{
	uint8 idata nAsrRes;
	uint8 i=0;
	Led_test();
	MCU_init();
	LD_Reset();
	UartIni(); /*串口初始化*/
	nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR
	
	#ifdef TEST	
  PrintCom("唤醒词:智能垃圾箱\r\n"); /*text.....*/
	PrintCom("      1、可回收垃圾\r\n"); /*text.....*/
	PrintCom("	2、干垃圾\r\n"); /*text.....*/
	PrintCom("	3、湿垃圾\r\n"); /*text.....*/
	PrintCom("	4、有害垃圾\r\n"); /*text.....*/
	PrintCom("      5、易拉罐\r\n"); /*text.....*/
	PrintCom("	6、报纸\r\n"); /*text.....*/
	PrintCom("	7、卫生纸\r\n"); /*text.....*/
	PrintCom("	8、毛巾\r\n"); /*text.....*/
	PrintCom("	9、贝剩菜剩饭\r\n"); /*text.....*/
	PrintCom("      10、过期食品\r\n"); /*text.....*/
	PrintCom("	11、电池\r\n"); /*text.....*/
	PrintCom("	12、杀虫剂\r\n"); /*text.....*/
	#endif
P10=1;P11=1;P12=1;P13=1;P14=1;
	while(1)
	{  
		switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:		
				break;
			case LD_ASR_NONE:
			{
				nAsrStatus=LD_ASR_RUNING;
				if (RunASR()==0)	/*	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算*/
				{
					nAsrStatus = LD_ASR_ERROR;
				}
				break;
			}
			case LD_ASR_FOUNDOK: /*	一次ASR识别流程结束，去取ASR识别结果*/
			{				
				nAsrRes = LD_GetResult();		/*获取结果*/
				User_handle(nAsrRes);//用户执行函数 
				nAsrStatus = LD_ASR_NONE;
				break;
			}
			case LD_ASR_FOUNDZERO:
			default:
			{
				nAsrStatus = LD_ASR_NONE;
				break;
			}
		}// switch	 			
	}// while

}
/***********************************************************
* 名    称： 	 LED灯测试
* 功    能： 单片机是否工作指示 					 
**********************************************************/
void Led_test(void)
{
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
	Delay200ms();
	LED=~ LED;
}
/***********************************************************
* 名    称： void MCU_init()
* 功    能： 单片机初始化 
**********************************************************/ 
void MCU_init()
{
	P1M1=0;P1M0=0XFF;
	
	P0 = 0xff;
	P1 = 0;
	P2 = 0xff;
	P3 = 0xff;
	P4 = 0xff;
	

	LD_MODE = 0;		//	设置MD管脚为低，并行模式读写
	IE0=1;
	EX0=1;
	EA=1;
}
/***********************************************************
* 名    称：	延时函数 
**********************************************************/ 
void Delay200us()		//@22.1184MHz
{
	unsigned char i, j;
	_nop_();
	_nop_();
	i = 5;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}

void  delay(unsigned long uldata)
{
	unsigned int j  =  0;
	unsigned int g  =  0;
	while(uldata--)
	Delay200us();
}
void delay_ms(unsigned int ms)
{                         
	unsigned int a;
	while(ms)
	{
		a=80;
		while(a--);
		ms--;
	}
	return;
}
void Delay200ms()		//@22.1184MHz
{
	unsigned char i, j, k;

	i = 17;
	j = 208;
	k = 27;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

/***********************************************************
* 名    称： 中断处理函数 
**********************************************************/ 
void ExtInt0Handler(void) interrupt 0  
{ 	
	ProcessInt0();				
}
/***********************************************************
* 名    称：用户执行函数 
* 功    能：识别成功后，执行动作可在此进行修改  					 
**********************************************************/
void 	User_handle(uint8 dat)
{
     //UARTSendByte(dat);//串口识别码（十六进制）
		 if(0==dat)
		 {
		  G0_flag=ENABLE;
			LED=0;
			PrintCom("收到\r\n"); /*text.....*/
			P14=0;delay_ms(1500);P14=1;
		 }
		 else if(ENABLE==G0_flag)
		 {	
		 		G0_flag=DISABLE;
				LED=1;
			 switch(dat)		   /*对结果执行相关操作,客户可删除Printcom 串口输出语句替换为其他需要控制的代码*/
			  {
				  case CODE_KHSLJ:			
						PrintCom("“可回收垃圾”命令识别成功\r\n"); /*text.....*/
						P10=0;delay_ms(2000);P10=1;
						break;
					case CODE_GLJ:	
						PrintCom("“干垃圾”命令识别成功\r\n"); /*text.....*/
						P11=0;delay_ms(2000);P11=1;
					  break;
					case CODE_SLJ:					
						PrintCom("“湿垃圾”命令识别成功\r\n"); /*text.....*/
						P12=0;delay_ms(2000);P12=1;
					  break;
					case CODE_YHLJ:											
						PrintCom("“有害垃圾”命令识别成功\r\n"); /*text.....*/
						P13=0;delay_ms(2000);P13=1;
						break;
					case CODE_1:						
						PrintCom("“易拉罐”命令识别成功\r\n"); /*text.....*/
						P10=0;delay_ms(2000);P10=1;
					  break;
					case CODE_2:						
						PrintCom("“报纸”命令识别成功\r\n"); /*text.....*/
						P10=0;delay_ms(2000);P10=1;
					  break;
					case CODE_3:					
						PrintCom("“卫生纸”命令识别成功\r\n"); /*text.....*/
						P11=0;delay_ms(2000);P11=1;
					  break;	
					case CODE_4:						
						PrintCom("“毛巾”命令识别成功\r\n"); /*text.....*/
						P11=0;delay_ms(2000);P11=1;
					  break;
					case CODE_5:						
						PrintCom("“剩菜剩饭”命令识别成功\r\n"); /*text.....*/
						P12=0;delay_ms(2000);P12=1;
					  break;
					case CODE_6:					
						PrintCom("“过期食品”命令识别成功\r\n"); /*text.....*/
						P12=0;delay_ms(2000);P12=1;
					  break;	
					case CODE_7:						
						PrintCom("“电池”命令识别成功\r\n"); /*text.....*/
						P13=0;delay_ms(2000);P13=1;
					  break;
					case CODE_8:						
						PrintCom("“杀虫剂”命令识别成功\r\n"); /*text.....*/
						P13=0;delay_ms(2000);P13=1;
					  break;					
					default:PrintCom("请重新识别发口令\r\n"); /*text.....*/break;
				}	
			}	
			else 	
			{
				PrintCom("请说出唤醒词\r\n"); /*text.....*/	
			}
}	 
