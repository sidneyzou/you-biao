/**********************����24C0Xϵ��ͷ�ļ�*********************************
*  ����:11.0592MHZ
*  2015��3��27�� 17:50:20
*  sda P2^0  scl P2^1
*  �ڴ� 0x0001-0x0fff  4k�汾
******************************************************************/
#ifndef __24C0X
#define __24C0X 1
/**********************�����ļ�**********************************************/
#include "HEAD.h"
#include <intrins.h>
/************************************�궨��************************************/

/************************************λ����************************************/
sbit eeprom_sda = P2^1;                          //IO�ڶ���
sbit eeprom_scl = P2^0;
/**********************************��������***********************************/

void EEPROM_Start(void);        //����I2C����
void EEPROM_Stop(void);         //ֹͣI2C����
void EEPROM_Clock(void);       //I2C����ʱ��
void EEPROM_Nop(void);  //ʱ����ʱ 2������
void EEPROM_Delay(uint m);  //��ȡд����ʱ
void EEPROM_Write_Byte(uchar dat);  //дһ���ֽ�
uchar EEPROM_Read_Byte(void);   //��һ���ֽ�

void EEPROM_Init(void);  //24c0X��ʼ���ӳ���  I2C���ߴ���
uchar EEPROM_Read(uchar add);  //��ȡʱ��
void EEPROM_Write(uchar add,uchar dat);//д��ʱ��
#endif