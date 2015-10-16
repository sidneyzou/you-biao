/**********************12864ͷ�ļ�*********************************
* ����:11.0592MHZ
* ���ݿ�P0
* RS  P1^0
* RW  P1^1
* EN  P2^5
* PSB P1^2
******************************************************************/
#ifndef __LCD
#define __LCD 1
/**********************�����ļ�**********************************************/
#include "HEAD.h"
#include <intrins.h>

/************************************�궨��************************************/
#define LCD_DATA  P0             //���ݿ�

#define LCD_DELAYNOP; {_nop_();_nop_();_nop_();_nop_();};
#define BASIC_SET 0x30
#define EXTEND_SET 0x34
#define DRAW_ON 0x36
#define DRAW_OFF 0x34
/************************************λ����************************************/
sbit LCD_RS  =  P1^0;            //�Ĵ���ѡ������ 
sbit LCD_RW  =  P1^1;            //Һ����/д����
sbit LCD_EN  =  P1^2;            //Һ��ʹ�ܿ���
sbit LCD_PSB =  P1^3;            //��/����ʽ����
/*********************************ȫ�ֱ��ʼ��***********************************/  

/**********************************��������***********************************/
void LCD_Delay(uint ms);       //��ʾ����ʱ 7.5ms *ms
void LCD_Delay_S(uint k);      //��ʱ 124us * k
bit LCD_Busy(void);            //��ʾ����æ
void LCD_Wcmd(uchar cmd);      //��ʾ��дָ��
void LCD_Wdat(uchar dat);      //��ʾ��д����
void LCD_Pos(uchar X,uchar Y); //��ʾ��������ʾλ��

void LCD_Wstring(uchar x,uchar y,uchar *Alphabet);
void LCD_Init(void);           //��ʾ����ʼ��
void LCD_Gui_Circle(uchar x0 , uchar y0 , uchar r);//��Բ
void LCD_Gui_Linewith(uchar x0 , uchar y0 , uchar x1 , uchar y1 , uchar with, uchar color);//��������
void LCD_Gui_line(uchar x0 , uchar y0 , uchar x1 , uchar y1 , uchar color);
void LCD_Gui_Rline(uchar x, uchar y0, uchar y1, uchar color);//��ֱ��
void LCD_Gui_Hline(uchar x0, uchar x1, uchar y, uchar color);//ˮƽ��
void LCD_Gui_Point_A(uchar x,uchar y);//������
void LCD_Gui_Point(uchar x,uchar y,uchar color);//�ڰ״��
void LCD_Gui_Clear();//��ͼ����
void LCD_Gui_Loadline(uchar x,uchar y,uchar *dat,uchar no,uchar flag);//���һ������
void LCD_Gui_Put_Area(uchar x,uchar y,uchar *dat,uchar hno,uchar lno,uchar flag);//����ͼ
void LCD_Wchar_Small(uchar x,uchar y,uchar dat);//5*8С�ֻ�ͼ
void LCD_Wstring_Small(uchar x,uchar y,uchar *dat);//5*8С���ַ���
#endif