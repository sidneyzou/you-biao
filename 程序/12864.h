/**********************12864头文件*********************************
* 晶振:11.0592MHZ
* 数据口P0
* RS  P1^0
* RW  P1^1
* EN  P2^5
* PSB P1^2
******************************************************************/
#ifndef __LCD
#define __LCD 1
/**********************包含文件**********************************************/
#include "HEAD.h"
#include <intrins.h>

/************************************宏定义************************************/
#define LCD_DATA  P0             //数据口

#define LCD_DELAYNOP; {_nop_();_nop_();_nop_();_nop_();};
#define BASIC_SET 0x30
#define EXTEND_SET 0x34
#define DRAW_ON 0x36
#define DRAW_OFF 0x34
/************************************位定义************************************/
sbit LCD_RS  =  P1^0;            //寄存器选择输入 
sbit LCD_RW  =  P1^1;            //液晶读/写控制
sbit LCD_EN  =  P1^2;            //液晶使能控制
sbit LCD_PSB =  P1^3;            //串/并方式控制
/*********************************全局变初始化***********************************/  

/**********************************函数定义***********************************/
void LCD_Delay(uint ms);       //显示屏延时 7.5ms *ms
void LCD_Delay_S(uint k);      //延时 124us * k
bit LCD_Busy(void);            //显示屏查忙
void LCD_Wcmd(uchar cmd);      //显示屏写指令
void LCD_Wdat(uchar dat);      //显示屏写数据
void LCD_Pos(uchar X,uchar Y); //显示屏设置显示位置

void LCD_Wstring(uchar x,uchar y,uchar *Alphabet);
void LCD_Init(void);           //显示屏初始化
void LCD_Gui_Circle(uchar x0 , uchar y0 , uchar r);//画圆
void LCD_Gui_Linewith(uchar x0 , uchar y0 , uchar x1 , uchar y1 , uchar with, uchar color);//任意连线
void LCD_Gui_line(uchar x0 , uchar y0 , uchar x1 , uchar y1 , uchar color);
void LCD_Gui_Rline(uchar x, uchar y0, uchar y1, uchar color);//竖直线
void LCD_Gui_Hline(uchar x0, uchar x1, uchar y, uchar color);//水平线
void LCD_Gui_Point_A(uchar x,uchar y);//任意打点
void LCD_Gui_Point(uchar x,uchar y,uchar color);//黑白打点
void LCD_Gui_Clear();//绘图清屏
void LCD_Gui_Loadline(uchar x,uchar y,uchar *dat,uchar no,uchar flag);//输出一行数据
void LCD_Gui_Put_Area(uchar x,uchar y,uchar *dat,uchar hno,uchar lno,uchar flag);//区域画图
void LCD_Wchar_Small(uchar x,uchar y,uchar dat);//5*8小字绘图
void LCD_Wstring_Small(uchar x,uchar y,uchar *dat);//5*8小字字符串
#endif