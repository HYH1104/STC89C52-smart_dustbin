/******************************************************
**  �������ƣ�UNV-01����ʶ��ģ����������
**	CPU: STC11L08XE
**	����22.1184MHZ
**	�����ʣ�9600 bit/S
***************************/
#include "config.h"
/************************************************************************************/
//	nAsrStatus ������main�������б�ʾ�������е�״̬������LD3320оƬ�ڲ���״̬�Ĵ���
//	LD_ASR_NONE:		��ʾû������ASRʶ��
//	LD_ASR_RUNING��		��ʾLD3320������ASRʶ����
//	LD_ASR_FOUNDOK:		��ʾһ��ʶ�����̽�������һ��ʶ����
//	LD_ASR_FOUNDZERO:	��ʾһ��ʶ�����̽�����û��ʶ����
//	LD_ASR_ERROR:		��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬
/***********************************************************************************/
uint8 idata nAsrStatus=0;	
void MCU_init(); 
void ProcessInt0(); //ʶ������
void delay(unsigned long uldata);
void User_handle(uint8 dat);//�û�ִ�в�������
void Delay200ms();
void Led_test(void);//��Ƭ������ָʾ
uint8_t G0_flag=DISABLE;//���б�־��ENABLE:���С�DISABLE:��ֹ���� 
sbit LED=P4^2;//�ź�ָʾ��
sbit P10=P1^0;
sbit P11=P1^1;
sbit P12=P1^2;
sbit P13=P1^3;
sbit P14=P1^4;

/***********************************************************
* ��    �ƣ� void  main(void)
* ��    �ܣ� ������	������� 
**********************************************************/ 
void  main(void)
{
	uint8 idata nAsrRes;
	uint8 i=0;
	Led_test();
	MCU_init();
	LD_Reset();
	UartIni(); /*���ڳ�ʼ��*/
	nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR
	
	#ifdef TEST	
  PrintCom("���Ѵ�:����������\r\n"); /*text.....*/
	PrintCom("      1���ɻ�������\r\n"); /*text.....*/
	PrintCom("	2��������\r\n"); /*text.....*/
	PrintCom("	3��ʪ����\r\n"); /*text.....*/
	PrintCom("	4���к�����\r\n"); /*text.....*/
	PrintCom("      5��������\r\n"); /*text.....*/
	PrintCom("	6����ֽ\r\n"); /*text.....*/
	PrintCom("	7������ֽ\r\n"); /*text.....*/
	PrintCom("	8��ë��\r\n"); /*text.....*/
	PrintCom("	9����ʣ��ʣ��\r\n"); /*text.....*/
	PrintCom("      10������ʳƷ\r\n"); /*text.....*/
	PrintCom("	11�����\r\n"); /*text.....*/
	PrintCom("	12��ɱ���\r\n"); /*text.....*/
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
				if (RunASR()==0)	/*	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����*/
				{
					nAsrStatus = LD_ASR_ERROR;
				}
				break;
			}
			case LD_ASR_FOUNDOK: /*	һ��ASRʶ�����̽�����ȥȡASRʶ����*/
			{				
				nAsrRes = LD_GetResult();		/*��ȡ���*/
				User_handle(nAsrRes);//�û�ִ�к��� 
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
* ��    �ƣ� 	 LED�Ʋ���
* ��    �ܣ� ��Ƭ���Ƿ���ָʾ 					 
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
* ��    �ƣ� void MCU_init()
* ��    �ܣ� ��Ƭ����ʼ�� 
**********************************************************/ 
void MCU_init()
{
	P1M1=0;P1M0=0XFF;
	
	P0 = 0xff;
	P1 = 0;
	P2 = 0xff;
	P3 = 0xff;
	P4 = 0xff;
	

	LD_MODE = 0;		//	����MD�ܽ�Ϊ�ͣ�����ģʽ��д
	IE0=1;
	EX0=1;
	EA=1;
}
/***********************************************************
* ��    �ƣ�	��ʱ���� 
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
* ��    �ƣ� �жϴ����� 
**********************************************************/ 
void ExtInt0Handler(void) interrupt 0  
{ 	
	ProcessInt0();				
}
/***********************************************************
* ��    �ƣ��û�ִ�к��� 
* ��    �ܣ�ʶ��ɹ���ִ�ж������ڴ˽����޸�  					 
**********************************************************/
void 	User_handle(uint8 dat)
{
     //UARTSendByte(dat);//����ʶ���루ʮ�����ƣ�
		 if(0==dat)
		 {
		  G0_flag=ENABLE;
			LED=0;
			PrintCom("�յ�\r\n"); /*text.....*/
			P14=0;delay_ms(1500);P14=1;
		 }
		 else if(ENABLE==G0_flag)
		 {	
		 		G0_flag=DISABLE;
				LED=1;
			 switch(dat)		   /*�Խ��ִ����ز���,�ͻ���ɾ��Printcom �����������滻Ϊ������Ҫ���ƵĴ���*/
			  {
				  case CODE_KHSLJ:			
						PrintCom("���ɻ�������������ʶ��ɹ�\r\n"); /*text.....*/
						P10=0;delay_ms(2000);P10=1;
						break;
					case CODE_GLJ:	
						PrintCom("��������������ʶ��ɹ�\r\n"); /*text.....*/
						P11=0;delay_ms(2000);P11=1;
					  break;
					case CODE_SLJ:					
						PrintCom("��ʪ����������ʶ��ɹ�\r\n"); /*text.....*/
						P12=0;delay_ms(2000);P12=1;
					  break;
					case CODE_YHLJ:											
						PrintCom("���к�����������ʶ��ɹ�\r\n"); /*text.....*/
						P13=0;delay_ms(2000);P13=1;
						break;
					case CODE_1:						
						PrintCom("�������ޡ�����ʶ��ɹ�\r\n"); /*text.....*/
						P10=0;delay_ms(2000);P10=1;
					  break;
					case CODE_2:						
						PrintCom("����ֽ������ʶ��ɹ�\r\n"); /*text.....*/
						P10=0;delay_ms(2000);P10=1;
					  break;
					case CODE_3:					
						PrintCom("������ֽ������ʶ��ɹ�\r\n"); /*text.....*/
						P11=0;delay_ms(2000);P11=1;
					  break;	
					case CODE_4:						
						PrintCom("��ë������ʶ��ɹ�\r\n"); /*text.....*/
						P11=0;delay_ms(2000);P11=1;
					  break;
					case CODE_5:						
						PrintCom("��ʣ��ʣ��������ʶ��ɹ�\r\n"); /*text.....*/
						P12=0;delay_ms(2000);P12=1;
					  break;
					case CODE_6:					
						PrintCom("������ʳƷ������ʶ��ɹ�\r\n"); /*text.....*/
						P12=0;delay_ms(2000);P12=1;
					  break;	
					case CODE_7:						
						PrintCom("����ء�����ʶ��ɹ�\r\n"); /*text.....*/
						P13=0;delay_ms(2000);P13=1;
					  break;
					case CODE_8:						
						PrintCom("��ɱ���������ʶ��ɹ�\r\n"); /*text.....*/
						P13=0;delay_ms(2000);P13=1;
					  break;					
					default:PrintCom("������ʶ�𷢿���\r\n"); /*text.....*/break;
				}	
			}	
			else 	
			{
				PrintCom("��˵�����Ѵ�\r\n"); /*text.....*/	
			}
}	 
