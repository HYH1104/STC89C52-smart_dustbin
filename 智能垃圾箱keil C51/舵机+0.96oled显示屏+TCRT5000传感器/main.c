//ͷ�ļ�����
#include "REG51.h"
#include "oled.h"
#include "intrins.h"
//�ܽŶ���
sbit TCRC = P0^0;
sbit YYSB1= P0^1;
sbit YYSB2= P0^2;
sbit YYSB3= P0^3;
sbit YYSB4= P0^4;
sbit YYSB5= P0^5;
sbit PWM1 = P3^4; 
sbit PWM2 = P3^5; 
sbit PWM3 = P3^6; 
sbit PWM4 = P3^7; 
//��������
unsigned char count = 0;
unsigned char timer1 ;
unsigned char timer2 ;
unsigned char timer3 ;
unsigned char timer4 ;
unsigned char cczd1 = 1;
unsigned char cczd2 = 1;
unsigned char cczd3 = 1;
unsigned char cczd4 = 1;
unsigned char UART_buff;
bit New_rec = 0, Send_ed = 1;
//��ʱ��0��ʼ��
void Timer0_Init()           
{
    TMOD &= 0x00;
    TMOD |= 0x21;
 
    TH0 = 0xff;// 0.1ms
    TL0 = 0xa4;
 
    ET0 = 1;      
    TR0 = 1; 
	  EA=1; 
}
//��ʱ��0�ж�
void Time0_Init() interrupt 1 
{
	TR0 = 0; 
	TH0 = 0xff; // 0.1ms
	TL0 = 0xa4;
	
	if(count <= timer1) //5==0��15==90��
	{ 
		PWM1 = 1; 
	}
	else 
	{ 
		PWM1 = 0; 
	}
	if(count <= timer2) //5==0?15==90?
	{ 
		PWM2 = 1; 
	}
	else 
	{ 
		PWM2 = 0; 
	}
	if(count <= timer3)//5==0��15==90��
	{ 
		PWM3 = 1; 
	}
	else 
	{ 
		PWM3 = 0; 
	}
	if(count <= timer4) //5==0��15==90��
	{ 
		PWM4 = 1; 
	}
	else 
	{ 
		PWM4 = 0; 
	}
	count++;
	if (count >= 200) //T = 20ms
	{ 
		count = 0; 
	}
	TR0 = 1; //��ʱ��T0
}
void se_int()
{
SCON = 0x50;   //���ڷ�ʽ1, 8-n-1, �������.
TH1 = 0xFD;    //[url=]//9600bps@11.0592MHz[/url]
TL1 = 0xFD;
IP=0x10;
TR1 = 1;                        
ES  = 1;       //���ж�.
}
//�����ж�
void ser_int (void) interrupt 4 
{
	Timer0_Init();
    if(RI == 1) 
	{  //����յ�.
    RI = 0;      //�����־.
    New_rec = 1;
    UART_buff = SBUF;  //����.
  if(UART_buff == '1') //���ڽ�������1�ӿ� 
		{
		cczd1=0;	
		}
	if(UART_buff == '2') //���ڽ�������2�ӿ� 
		{		
		cczd2=0;
		}
	if(UART_buff == '3')  //���ڽ�������3�ӿ�
		{		
		cczd3=0;	
		}
	if(UART_buff == '4')  //���ڽ�������4�ӿ�
		{		
		cczd4=0;
		}
    }
    else  
	{        //����ͱ�.
    TI = 0;      //�����־.
    Send_ed = 1;
    }
}
//������
int main(void)
{	
//��ʼ��
OLED_Init();	
Timer0_Init();
se_int();
TCRC=1;YYSB1=1;YYSB2=1;YYSB3=1;YYSB4=1;
// ��ʼ����
OLED_ShowCHinese(24,0, 0);
OLED_ShowCHinese(40,0, 1);
OLED_ShowCHinese(56,0, 2);
OLED_ShowCHinese(72,0, 3);
OLED_ShowCHinese(88,0, 4);	
OLED_ShowString	(28,2,"programer:");
OLED_ShowCHinese(36,4, 8);
OLED_ShowCHinese(54,4, 9);
OLED_ShowCHinese(72,4,10);
delay_ms(3000);
OLED_Clear();
//��ѭ��	 
 while(1) 
 {	  	
		int pd=1;
		if(TCRC==0)//TCRC���⴫�нӿ�
		{
		OLED_Clear();
		OLED_ShowCHinese(40,2,11);
		OLED_ShowCHinese(72,2,12);
		delay_ms(500);
		timer1 =14;//���1��
		count=0;
		delay_ms(500);
		OLED_Clear();
		OLED_ShowCHinese(36,2,27);
		OLED_ShowCHinese(54,2,11);
		OLED_ShowCHinese(72,2,12);	
			while(pd>0)
			{
				if(TCRC==1)
				{
				OLED_Clear();
				OLED_ShowCHinese(40,2,20);
				OLED_ShowCHinese(72,2,21);
				delay_ms(500);
				OLED_Clear();	
				timer1 =6;//���1��
				count=0;	
				TCRC=1;
				delay_ms(500);
				OLED_Clear();
				pd=-1;
				}
			}
		}		
		if((YYSB1==0)||(cczd1==0))//����ʶ��1�ӿ�
		{
		OLED_Clear();
		OLED_ShowCHinese(24,2,16);
	  OLED_ShowCHinese(40,2,17);
	 	OLED_ShowCHinese(56,2,18);
		OLED_ShowCHinese(72,2, 2);
		OLED_ShowCHinese(88,2, 3);	
		timer1 =14;//���1��
		count=0;
		delay_ms(3500);
		timer1 =6;//���1��
		count=0;
		YYSB1=1;
		cczd1=1;
		OLED_Clear();
		}		
		if((YYSB2==0)||(cczd2==0))//����ʶ��2�ӿ�
		{
		OLED_Clear();
		OLED_ShowCHinese(36,2,5);
		OLED_ShowCHinese(54,2,2);
		OLED_ShowCHinese(72,2,3);			
		timer2 =14;//���2��
		count=0;
		delay_ms(3500);
		timer2 =6;//���2��
		count=0;
		YYSB2=1;
		cczd2=1;
		OLED_Clear();
		}	
		if((YYSB3==0)||(cczd3==0))//����ʶ��3�ӿ�
		{
		OLED_Clear();
		OLED_ShowCHinese(36,2,19);
		OLED_ShowCHinese(54,2, 2);
		OLED_ShowCHinese(72,2, 3);	
		timer3 =14;//���3��
		count=0;
		delay_ms(3500);
		timer3 =6;//���3��
		count=0;
		YYSB3=1;
		cczd3=1;
  	OLED_Clear();
		}
		if((YYSB4==0)||(cczd4==0))//����ʶ��4�ӿ�
		{
		OLED_Clear();
		OLED_ShowCHinese(32,2,6);
		OLED_ShowCHinese(48,2,7);
		OLED_ShowCHinese(64,2,2);
		OLED_ShowCHinese(80,2,3);	
		timer4 =14;//���4��
		count=0;
		delay_ms(3500);
		timer4 =6;//���4��
		count=0;
		YYSB4=1;
		cczd4=1;
		OLED_Clear();
		}	
		if((TCRC=1)&&(YYSB1==1)&&(YYSB2==1)&&(YYSB3==1)&&(YYSB4==1)&&(YYSB5==1))//����ʶ��5�ӿ�
		{
		OLED_ShowCHinese(40,2,25);
		OLED_ShowCHinese(72,2,26);
		}
		else//�ǹ���
		{
		OLED_Clear();
		OLED_ShowCHinese(36,2,22);
		OLED_ShowCHinese(54,2,23);
		OLED_ShowCHinese(72,2,24);
		delay_ms(666);	
		OLED_Clear();			
		}
	}
 }		