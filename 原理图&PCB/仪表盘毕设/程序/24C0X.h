/**********************外扩24C0X系列头文件*********************************
*  晶振:11.0592MHZ
*  2015年3月27日 17:50:20
*  sda P2^0  scl P2^1
*  内存 0x0001-0x0fff  4k版本
******************************************************************/
#ifndef __24C0X
#define __24C0X 1
/**********************包含文件**********************************************/
#include "HEAD.h"
#include <intrins.h>
/************************************宏定义************************************/

/************************************位定义************************************/
sbit eeprom_sda = P2^1;                          //IO口定义
sbit eeprom_scl = P2^0;
/**********************************函数定义***********************************/

void EEPROM_Start(void);        //启动I2C总线
void EEPROM_Stop(void);         //停止I2C总线
void EEPROM_Clock(void);       //I2C总线时钟
void EEPROM_Nop(void);  //时序延时 2个周期
void EEPROM_Delay(uint m);  //读取写入延时
void EEPROM_Write_Byte(uchar dat);  //写一个字节
uchar EEPROM_Read_Byte(void);   //读一个字节

void EEPROM_Init(void);  //24c0X初始化子程序  I2C总线传输
uchar EEPROM_Read(uchar add);  //读取时序
void EEPROM_Write(uchar add,uchar dat);//写入时序
#endif