

#ifndef REG_RW_H
#define REG_RW_H

//�����˿ڶ���
sbit LD_MODE	=P4^3; /*��дģʽѡ��*/
sbit RSTB=P3^5;	 /*��λ�˿�*/
sbit CSB=P2^1;	 /*ģ��Ƭѡ�˿�*/


//��������
void LD_WriteReg( unsigned char address, unsigned char dataout );
unsigned char LD_ReadReg( unsigned char address );


#endif
