/***********************************************
**  工程名称：UNV-01语音识别模块驱动程序
**	CPU: STC11L08XE
**	晶振：22.1184MHZ
**	波特率：9600 bit/S
/*****************************************************/
#include "config.h"

		#define LD_INDEX_PORT		(*((volatile unsigned char xdata*)(0x8100))) 
		#define LD_DATA_PORT		(*((volatile unsigned char xdata*)(0x8000))) 
		//评估板上 MCU的A8 连接到 LD芯片的AD
		//         MCU的A14 连接到 LD芯片的CSB
		//         MCU的RD、WR 连接 LD芯片的RD、WR (xdata 读写时自动产生低信号)
		//
		//0x8100的二进制是10000001 00000000		CSB=0 AD=1
		//                 ^     ^
		//0x8000的二进制是10000000 00000000		CSB=0 AD=0
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

