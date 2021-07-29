/***********************************************
**  �������ƣ�UNV-01����ʶ��ģ����������
**	CPU: STC11L08XE
**	����22.1184MHZ
**	�����ʣ�9600 bit/S
/*****************************************************/
#include "config.h"

		#define LD_INDEX_PORT		(*((volatile unsigned char xdata*)(0x8100))) 
		#define LD_DATA_PORT		(*((volatile unsigned char xdata*)(0x8000))) 
		//�������� MCU��A8 ���ӵ� LDоƬ��AD
		//         MCU��A14 ���ӵ� LDоƬ��CSB
		//         MCU��RD��WR ���� LDоƬ��RD��WR (xdata ��дʱ�Զ��������ź�)
		//
		//0x8100�Ķ�������10000001 00000000		CSB=0 AD=1
		//                 ^     ^
		//0x8000�Ķ�������10000000 00000000		CSB=0 AD=0
		//                 ^     ^		
		void LD_WriteReg( unsigned char address, unsigned char dataout )
		{
			LD_INDEX_PORT  = address;         
			LD_DATA_PORT = dataout;          
		}
		
		unsigned char LD_ReadReg( unsigned char address )
		{
			LD_INDEX_PORT = address;         
			return (unsigned char)LD_DATA_PORT;     
		}

