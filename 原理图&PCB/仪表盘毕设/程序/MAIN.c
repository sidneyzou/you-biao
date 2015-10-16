/**********************�������ļ�*******************
*  ����:11.0592MHZ
*  
******************************************************************/
/**********************�����ļ�*******************/
#include <reg52.h>
#include "HEAD.h"
#include "12864.h"
#include <math.h>
#include "24C0X.h"
#include "DS18B20.h"
#include "KEY.h"
#include "PWM.h"
/*********************************ȫ�ֱ�������***********************************/
uchar show = 0x01;//  8�¶� 7��� 6ת�� 5ʱ�� 4��� 3��һ 2ʱ�� 1ȫ��  0��
double trip_data = 0;
uint mps_data = 0;
sbit IN3 = P3^5;
sbit IN4 = P3^6;
sbit BEEP = P2^3;
/*****************************************������*************************************/

void main(void)
{
	uchar x1,y1,x2,y2,n1 = 0,n2 = 0,n1_last = 0,n2_last = 0;
	uchar YMD[] = "2015:05:08";//0-3 year 5-6 month 8-9 day
	uchar Time[] = "14:06";//0-1 hour 3-4 minute
	uchar Temp[] = "00";
	uchar Mps[] = "0000";
	uchar Rs[] = "000";
	uchar Trip[] = "0000";
	uchar i = 0;	
	//�¶�����2λ ��     �ٶ�����4λ m/h    ת������3λ r/m    �������4λ m
	uint tem_1 = 0,tem_2 = 0;
	uint Data_Show_Last = 0;
	uchar hour = 0,minute = 0;
	BEEP = 0;//�رշ�����
	
	LCD_Init();  
	//LCD_Clear();
	LCD_Gui_Clear();
	IN3 = 1;
	IN4 = 0;
	//ENA = 1;
	//USER_Timer_Init();
	PWM_Init_0();
	pwm_set_0 = 3000;
	pwm_cycle_0 = 10000;
	USER_Timer_Init();
	EEPROM_Init();
	/***************************************************************************/
	Trip[0] = EEPROM_Read(0x04);//���λ
	Trip[1] = EEPROM_Read(0x03);
	Trip[2] = EEPROM_Read(0x02);
	Trip[3] = EEPROM_Read(0x01);
	trip_data = (Trip[0]-0x30) * 1000 + (Trip[1]-0x30) * 100 + (Trip[2]-0x30) * 10 + (Trip[3]-0x30);//���
	show |= 0x40;
	//show |= 0x30;
	minute = (Time[4] - 0x30) + (Time[3] - 0x30) * 10;
	hour = (Time[1] - 0x30) + (Time[0] - 0x30) * 10;
	/*************************������***************************************
	EEPROM_Write(0x01,0x30);
	EEPROM_Write(0x02,0x30);
	EEPROM_Write(0x03,0x30);
	EEPROM_Write(0x04,0x30);
	/***************************************************************************/
	EA = 1;
	//EEPROM_Init();
	//��ʼ��ȫ��
	while(1)
	{
		/******************���̼�����***************************/
		KEY_Scan();
		/******************ת�ٽ���***************************/
		if(Data_Show_Last != Data_Show && show != 0x01)
		{
			Rs[0] = Data_Show / 100 % 10 + 0x30;//ת��
			Rs[1] = Data_Show / 10 % 10 + 0x30;
			Rs[2] = Data_Show % 10 + 0x30;
			Data_Show_Last = Data_Show;
			show |= 0x20;
			n2 = (Data_Show / 35);//��2
			if(n2_last != n2)
			{
				show |= 0x08;
				n2_last = n2;
			}
			
			mps_data = 21.98 * Data_Show * 60 / 100;//ʱ�� m/h
			Mps[0] = mps_data / 1000 % 10 + 0x30;
			Mps[1] = mps_data / 100 % 10 + 0x30;
			Mps[2] = mps_data / 10 % 10 + 0x30;
			Mps[3] = mps_data % 10 + 0x30;
			show |= 0x10;
			n1 = (mps_data / 450); //��1
			if(n1_last != n1)
			{
				show |= 0x04;
				n1_last = n1;
			}
			
			//trip_data += mps_data / 60 / 1000;
		}
		/******************ʱ�����̼���*******************************/
		if(user_time > 60)
		{
			user_time = 0;
			Trip[0] = (uint)trip_data / 1000 % 10 + 0x30;//���λ
			Trip[1] = (uint)trip_data / 100 % 10 + 0x30;
			Trip[2] = (uint)trip_data / 10 % 10 + 0x30;
			Trip[3] = (uint)trip_data % 10 + 0x30;
			EEPROM_Write(0x01,Trip[3]);
			EEPROM_Write(0x02,Trip[2]);
			EEPROM_Write(0x03,Trip[1]);
			EEPROM_Write(0x04,Trip[0]);
			show = 0x40;
			minute++;
			if(minute == 60)
			{
				minute = 0;
				hour++;
				Time[4] = '0';
				Time[3] = '0';
				Time[1] = hour % 10 + 0x30;
				Time[0] = hour / 10 + 0x30;
			}else
			{
				Time[4] = minute % 10 + 0x30;
				Time[3] = minute / 10 + 0x30;
			}
			show |= 0x02;
		}
		/******************�¶Ƚ���***************************/
		tem_1 = TMP();
		if(tem_2 != tem_1 && tem_1 < 500)
		{
			Temp[0] = tem_1 / 100 % 10+ 0x30;
			Temp[1] = tem_1 / 10 % 10 + 0x30;
			//Temp[0] = 0x32;
			//Temp[1] = 0x32;
			tem_2 = tem_1;
			//LCD_Gui_Point(1,1,1);
			if(tem_1 > 350)//35�ȱ���
				BEEP = 1;
			else BEEP = 0;
			show |= 0x80;
		}
		/****************************************************
		LCD_Wstring(0,0,"ABC");
		
		/******************ͼ�λ���***************************/
		if( (show & 0x01) == 0x01)
		{
			LCD_Wstring_Small(25,0,YMD);
			LCD_Wstring_Small(80,0,Time);
			
			LCD_Gui_Circle(35,27,18);//��һ������
			LCD_Wstring_Small(16,37,"0");//��ʼ�̶�
			LCD_Wstring_Small(51,37,"4000");//��ֹ�̶�
			y1 = 27 + cos(3.14 / 6 * n1 + 3.14 / 4) * 18;
			x1 = 35 - sin(3.14 / 6 * n1 + 3.14 / 4) * 18;//��1�Ƕ��㷨
			LCD_Gui_line(35,27,x1,y1,1);
			
			LCD_Gui_Circle(95,27,18);//�ڶ�������
			LCD_Wstring_Small(76,37,"0");//��ʼ�̶�
			LCD_Wstring_Small(76+35,37,"300");//��ֹ�̶�
			y2 = 27 + cos(3.14 / 6 * n2 + 3.14 / 4) * 18;
			x2 = 95 - sin(3.14 / 6 * n2 + 3.14 / 4) * 18;//��2�Ƕ��㷨
			LCD_Gui_line(95,27,x2,y2,1);
			
			//��һ�� ʱ�� ת��
			LCD_Wstring_Small(0,47,"MPS :");//ʱ��
			//LCD_Wstring_Small(25+5,47,Mps);
			LCD_Wstring_Small(46,47,"m/h");
			
			LCD_Wstring_Small(58+10,47,"RS:");//ת��
			//LCD_Wstring_Small(72+15,47,Rs);
			LCD_Wstring_Small(82+20,47,"r/m");
			
			//�ڶ��� ��� �¶�
			LCD_Wstring_Small(0,56,"TRIP:");//���
			//LCD_Wstring_Small(25+5,56,Trip);
			LCD_Wstring_Small(46,56,"m");
			
			LCD_Wstring_Small(50+18,56,"T :");//�¶�
			//LCD_Wstring_Small(60+27,56,Temp);
			LCD_Wstring_Small(70+27,56,"C");
			show &= 0xfe;
			//USER_Timer_Init();
			//EA = 1;
		}
		
		if( (show & 0x02) == 0x02)//ʱ��
		{
			LCD_Wstring_Small(80,0,Time);//show 2 time
			show &= 0xfd;
		}
		if( (show & 0x04) == 0x04)
		{
			LCD_Gui_line(35,27,x1,y1,0);//show 3 biao 1 ����
			//LCD_Gui_Circle(35,27,18);//��һ������
			//LCD_Wstring_Small(16,37,"0");//��ʼ�̶�
			//LCD_Wstring_Small(51,37,"90");//��ֹ�̶�
			y1 = 27 + cos(3.14 / 6 * n1 + 3.14 / 4) * 18;
			x1 = 35 - sin(3.14 / 6 * n1 + 3.14 / 4) * 18;//��1�Ƕ��㷨
			LCD_Gui_line(35,27,x1,y1,1);//show 3 biao 1
			show &= 0xfb;
		}
		if( (show & 0x08) == 0x08)
		{
			LCD_Gui_line(95,27,x2,y2,0);//show 4 biao 2 ����
			//LCD_Gui_Circle(95,27,18);//�ڶ�������
			//LCD_Wstring_Small(76,37,"0");//��ʼ�̶�
			//LCD_Wstring_Small(76+35,37,"90");//��ֹ�̶�
			y2 = 27 + cos(3.14 / 6 * n2 + 3.14 / 4 ) * 18;
			x2 = 95 - sin(3.14 / 6 * n2 + 3.14 / 4 ) * 18;//��2�Ƕ��㷨
			LCD_Gui_line(95,27,x2,y2,1);//show 4 biao 2
			show &= 0xf7;
		}
		if( (show & 0x10) == 0x10)//ʱ�� MPS
		{
			LCD_Wstring_Small(25,47,Mps);//show 5 MPS
			show &= 0xef;
		}
		if( (show & 0x20) == 0x20)//ת�� RS
		{
			LCD_Wstring_Small(72+15,47,Rs);//show 6 RS
			show &= 0xdf;
		}
		if( (show & 0x40) == 0x40)//��� TRIP
		{
			LCD_Wstring_Small(25,56,Trip);//show 7 TRIP
			show &= 0xbf;
		}
		if( (show & 0x80) == 0x80)//�¶� tempture
		{
			LCD_Wstring_Small(60+27,56,Temp);//show 8 TEMP
			show &= 0x7f;
		}
	/***************************************************************/
	
	}
}