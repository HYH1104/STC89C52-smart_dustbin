/***********************************************
**  �������ƣ�UNV-LD3320+STC11����ʶ��ģ����������
**	CPU: STC11L08XE
**	����22.1184MHZ
**	�����ʣ�9600 bit/S
/***********************************************/
#include "config.h"
#include "stdio.h"
extern void  delay(unsigned long uldata);

uint8 idata ucRegVal;
extern uint8 idata nAsrStatus;

void ProcessInt0(void);

/************************************************************************
���������� 	 ��λLDģ��
**************************************************************************/
void LD_Reset()
{
	char xdata temp_data[5],res=0,len=0;
	RSTB=1;
	delay(5);
	RSTB=0;
	delay(5);
	RSTB=1;

	delay(5);
	CSB=0;
	delay(5);
	CSB=1;
	delay(5);
	
	
	res=LD_ReadReg(0x6);
	len+=sprintf(temp_data+len,"REG-06:%x",res);
	LD_WriteReg(0x35, 0x33);
	
	LD_WriteReg(0x1b, 0x55);
	LD_WriteReg(0xb3, 0xaa);
	
	res=LD_ReadReg(0x35);
	len+=sprintf(temp_data+len,"REG-35:%x",res);
	res=LD_ReadReg(0x1b);
	len+=sprintf(temp_data+len,"REG-1b:%x",res);
	res=LD_ReadReg(0xb3);
	len+=sprintf(temp_data+len,"REG-b3:%x",res);
		
	PrintCom(temp_data);
}
/************************************************************************
���������� LDģ�������ʼ��
��ڲ����� none
�� �� ֵ�� none
**************************************************************************/
void LD_Init_Common()
{
	LD_ReadReg(0x06);  
	LD_WriteReg(0x17, 0x35); 
	delay(10);
	LD_ReadReg(0x06);  

	LD_WriteReg(0x89, 0x03);  
	delay(5);
	LD_WriteReg(0xCF, 0x43);   
	delay(5);
	LD_WriteReg(0xCB, 0x02);
	
	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);       

	LD_WriteReg(0x1E,0x00);
	LD_WriteReg(0x19, LD_PLL_ASR_19); 
	LD_WriteReg(0x1B, LD_PLL_ASR_1B);		
  LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	delay(10);
	
    LD_WriteReg(0xCD, 0x04);
//	LD_WriteReg(0x17, 0x4c); 
	delay(5);
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F); 
	LD_WriteReg(0x6F, 0xFF); 
}

/************************************************************************
���������� 	 LDģ�� ASR���ܳ�ʼ��
**************************************************************************/
void LD_Init_ASR()
{
	LD_Init_Common();
	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);
	delay( 10 );
	LD_WriteReg(0x3C, 0x80);    
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff);    
	LD_WriteReg(0x3A, 0x07);
  LD_WriteReg(0x40, 0);          
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0);    
	LD_WriteReg(0x46, 8); 
	delay( 1 );
}

/************************************************************************
���������� 	�жϴ�����
**************************************************************************/
void ProcessInt0(void)
{
	uint8 nAsrResCount=0;

	EX0=0;
	ucRegVal = LD_ReadReg(0x2B);
	LD_WriteReg(0x29,0) ;
	LD_WriteReg(0x02,0) ;
	if((ucRegVal & 0x10) &&
		LD_ReadReg(0xb2)==0x21 && 
		LD_ReadReg(0xbf)==0x35)			/*ʶ��ɹ�*/
	{	
		nAsrResCount = LD_ReadReg(0xba);
		if(nAsrResCount>0 && nAsrResCount<=4) 
		{
			nAsrStatus=LD_ASR_FOUNDOK;
		}
		else
	    {
			nAsrStatus=LD_ASR_FOUNDZERO;
		}	
	}															 /*û��ʶ����*/
	else
	{	 
		nAsrStatus=LD_ASR_FOUNDZERO;
	}
		
  LD_WriteReg(0x2b, 0);
  LD_WriteReg(0x1C,0);/*д0:ADC������*/

	LD_WriteReg(0x29,0) ;
	LD_WriteReg(0x02,0) ;
	LD_WriteReg(0x2B,  0);
	LD_WriteReg(0xBA, 0);	
	LD_WriteReg(0xBC,0);	
	LD_WriteReg(0x08,1);	 /*���FIFO_DATA*/
	LD_WriteReg(0x08,0);	/*���FIFO_DATA�� �ٴ�д0*/


	EX0=1;
}

/************************************************************************
���������� 	����ASRʶ������
��ڲ�����	none
�� �� ֵ��  asrflag��1->�����ɹ��� 0��>����ʧ��
����˵����	ʶ��˳������:
						1��RunASR()����ʵ����һ��������ASR����ʶ������
						2��LD_AsrStart() ����ʵ����ASR��ʼ��
						3��LD_AsrAddFixed() ����ʵ������ӹؼ����ﵽLD3320оƬ��
						4��LD_AsrRun()	����������һ��ASR����ʶ������					
						�κ�һ��ASRʶ�����̣�����Ҫ�������˳�򣬴ӳ�ʼ����ʼ��
**************************************************************************/
uint8 RunASR(void)
{
	uint8 i=0;
	uint8 asrflag=0;
	for (i=0; i<5; i++)			//	��ֹ����Ӳ��ԭ����LD3320оƬ����������������һ������5������ASRʶ������
	{
		LD_AsrStart();
		delay(50);
		if (LD_AsrAddFixed()==0)
		{
			LD_Reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			delay(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
			continue;
		}
		delay(10);
		if (LD_AsrRun() == 0)
		{
			LD_Reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			delay(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
			continue;
		}
		asrflag=1;
		break;					//	ASR���������ɹ����˳���ǰforѭ������ʼ�ȴ�LD3320�ͳ����ж��ź�
	}

	return asrflag;
}
/************************************************************************
����������  ���LDģ���Ƿ����
�� �� ֵ�� 	flag��1-> ����
**************************************************************************/
uint8 LD_Check_ASRBusyFlag_b2()
{
	uint8 j;
	uint8 flag = 0;
	for (j=0; j<10; j++)
	{
		if (LD_ReadReg(0xb2) == 0x21)
		{
			flag = 1;
			break;
		}
		delay(10);		
	}
	return flag;
}
/************************************************************************
���������� 	����ASR
**************************************************************************/
void LD_AsrStart()
{
	LD_Init_ASR();
}
/************************************************************************
���������� 	����ASR
�� �� ֵ�� 	1�������ɹ�
**************************************************************************/
uint8 LD_AsrRun()
{
	EX0=0;
	LD_WriteReg(0x35, MIC_VOL);
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);
	delay( 1 );
	LD_WriteReg(0x08, 0x00);
	delay( 1 );

	if(LD_Check_ASRBusyFlag_b2() == 0)
	{
		return 0;
	}
//	LD_WriteReg(0xB6, 0xa); //ʶ��ʱ��	 1S
//	LD_WriteReg(0xB5, 0x1E); //��������ʱ�� 300ms
//	LD_WriteReg(0xB8, 10); //����ʱ��

//	LD_WriteReg(0x1C, 0x07); //����˫ͨ����Ƶ�ź���Ϊ�����ź�
	LD_WriteReg(0x1C, 0x0b); //������˷���Ϊ�����ź�


	LD_WriteReg(0xB2, 0xff);
	delay( 1);	
	LD_WriteReg(0x37, 0x06);
	delay( 1 );
    LD_WriteReg(0x37, 0x06);
		delay( 5 );
	LD_WriteReg(0x29, 0x10);
	
	LD_WriteReg(0xBD, 0x00);
	EX0=1;
	return 1;
}
/************************************************************************
���������� ��LDģ����ӹؼ���
��ڲ����� none
�� �� ֵ�� flag��1->��ӳɹ�
**************************************************************************/
uint8 LD_AsrAddFixed()
{
	uint8 k, flag;
	uint8 nAsrAddLength;
	#define DATE_A 50   /*�����ά��ֵ*/
	#define DATE_B 30		/*����һά��ֵ*/
	uint8 code sRecog[DATE_A][DATE_B] = {
																				"zhi neng la ji xiang",\		
																				 "ke hui shou la ji",\
																				 "gan la ji",\
																				 "shi la ji",\
																				 "you hai la ji",\
																				 "yi la guan",\
																				 "bao zhi",\
																				 "wei sheng zhi",\
																				 "mao jin",\
																				 "sheng cai sheng fan",\
																				 "guo qi shi pin",\
																				 "dian chi",\
																				 "sha chong ji"	
																			};	/*��ӹؼ��ʣ��û��޸�*/
	uint8 code pCode[DATE_A] = {
															CODE_CMD,\
															CODE_KHSLJ,\
															CODE_GLJ,\
															CODE_SLJ,\
															CODE_YHLJ,\
															CODE_1,\
															CODE_2,\
															CODE_3,\
															CODE_4,\
															CODE_5,\
															CODE_6,\
															CODE_7,\
															CODE_8
														 };	/*���ʶ���룬�û��޸�*/	
	flag = 1;
	for (k=0; k<DATE_A; k++)
	{
			
		if(LD_Check_ASRBusyFlag_b2() == 0)
		{
			flag = 0;
			break;
		}
		
		LD_WriteReg(0xc1, pCode[k] );
		LD_WriteReg(0xc3, 0 );
		LD_WriteReg(0x08, 0x04);
		delay(1);
		LD_WriteReg(0x08, 0x00);
		delay(1);

		for (nAsrAddLength=0; nAsrAddLength<DATE_B; nAsrAddLength++)
		{
			if (sRecog[k][nAsrAddLength] == 0)
				break;
			LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
		}
		LD_WriteReg(0xb9, nAsrAddLength);
		LD_WriteReg(0xb2, 0xff);
		LD_WriteReg(0x37, 0x04);
	}
    return flag;
}
/************************************************************************
���������� 	��ȡʶ����
�� �� ֵ�� 	LD_ReadReg(0xc5 )��  ��ȡ�ڲ��Ĵ�������ʶ���롣
**************************************************************************/
uint8 LD_GetResult()
{		
	return LD_ReadReg(0xc5 );
}



