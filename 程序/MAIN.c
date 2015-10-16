/**********************主函数文件*******************
*  晶振:11.0592MHZ
*  
******************************************************************/
/**********************包含文件*******************/
#include <reg52.h>
#include "HEAD.h"
#include "12864.h"
#include <math.h>
#include "24C0X.h"
#include "DS18B20.h"
#include "KEY.h"
#include "PWM.h"
/*********************************全局变量定义***********************************/
uchar show = 0x01;//  8温度 7里程 6转速 5时速 4表二 3表一 2时间 1全清  0无
double trip_data = 0;
uint mps_data = 0;
sbit IN3 = P3^5;
sbit IN4 = P3^6;
sbit BEEP = P2^3;
/*****************************************主函数*************************************/

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
	//温度数据2位 度     速度数据4位 m/h    转速数据3位 r/m    里程数据4位 m
	uint tem_1 = 0,tem_2 = 0;
	uint Data_Show_Last = 0;
	uchar hour = 0,minute = 0;
	BEEP = 0;//关闭蜂鸣器
	
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
	Trip[0] = EEPROM_Read(0x04);//最大位
	Trip[1] = EEPROM_Read(0x03);
	Trip[2] = EEPROM_Read(0x02);
	Trip[3] = EEPROM_Read(0x01);
	trip_data = (Trip[0]-0x30) * 1000 + (Trip[1]-0x30) * 100 + (Trip[2]-0x30) * 10 + (Trip[3]-0x30);//里程
	show |= 0x40;
	//show |= 0x30;
	minute = (Time[4] - 0x30) + (Time[3] - 0x30) * 10;
	hour = (Time[1] - 0x30) + (Time[0] - 0x30) * 10;
	/*************************清空里程***************************************
	EEPROM_Write(0x01,0x30);
	EEPROM_Write(0x02,0x30);
	EEPROM_Write(0x03,0x30);
	EEPROM_Write(0x04,0x30);
	/***************************************************************************/
	EA = 1;
	//EEPROM_Init();
	//初始化全部
	while(1)
	{
		/******************键盘检测接收***************************/
		KEY_Scan();
		/******************转速接收***************************/
		if(Data_Show_Last != Data_Show && show != 0x01)
		{
			Rs[0] = Data_Show / 100 % 10 + 0x30;//转速
			Rs[1] = Data_Show / 10 % 10 + 0x30;
			Rs[2] = Data_Show % 10 + 0x30;
			Data_Show_Last = Data_Show;
			show |= 0x20;
			n2 = (Data_Show / 35);//表2
			if(n2_last != n2)
			{
				show |= 0x08;
				n2_last = n2;
			}
			
			mps_data = 21.98 * Data_Show * 60 / 100;//时速 m/h
			Mps[0] = mps_data / 1000 % 10 + 0x30;
			Mps[1] = mps_data / 100 % 10 + 0x30;
			Mps[2] = mps_data / 10 % 10 + 0x30;
			Mps[3] = mps_data % 10 + 0x30;
			show |= 0x10;
			n1 = (mps_data / 450); //表1
			if(n1_last != n1)
			{
				show |= 0x04;
				n1_last = n1;
			}
			
			//trip_data += mps_data / 60 / 1000;
		}
		/******************时间和里程计算*******************************/
		if(user_time > 60)
		{
			user_time = 0;
			Trip[0] = (uint)trip_data / 1000 % 10 + 0x30;//最大位
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
		/******************温度接收***************************/
		tem_1 = TMP();
		if(tem_2 != tem_1 && tem_1 < 500)
		{
			Temp[0] = tem_1 / 100 % 10+ 0x30;
			Temp[1] = tem_1 / 10 % 10 + 0x30;
			//Temp[0] = 0x32;
			//Temp[1] = 0x32;
			tem_2 = tem_1;
			//LCD_Gui_Point(1,1,1);
			if(tem_1 > 350)//35度报警
				BEEP = 1;
			else BEEP = 0;
			show |= 0x80;
		}
		/****************************************************
		LCD_Wstring(0,0,"ABC");
		
		/******************图形绘制***************************/
		if( (show & 0x01) == 0x01)
		{
			LCD_Wstring_Small(25,0,YMD);
			LCD_Wstring_Small(80,0,Time);
			
			LCD_Gui_Circle(35,27,18);//第一个表盘
			LCD_Wstring_Small(16,37,"0");//初始刻度
			LCD_Wstring_Small(51,37,"4000");//终止刻度
			y1 = 27 + cos(3.14 / 6 * n1 + 3.14 / 4) * 18;
			x1 = 35 - sin(3.14 / 6 * n1 + 3.14 / 4) * 18;//表1角度算法
			LCD_Gui_line(35,27,x1,y1,1);
			
			LCD_Gui_Circle(95,27,18);//第二个表盘
			LCD_Wstring_Small(76,37,"0");//初始刻度
			LCD_Wstring_Small(76+35,37,"300");//终止刻度
			y2 = 27 + cos(3.14 / 6 * n2 + 3.14 / 4) * 18;
			x2 = 95 - sin(3.14 / 6 * n2 + 3.14 / 4) * 18;//表2角度算法
			LCD_Gui_line(95,27,x2,y2,1);
			
			//第一排 时速 转速
			LCD_Wstring_Small(0,47,"MPS :");//时速
			//LCD_Wstring_Small(25+5,47,Mps);
			LCD_Wstring_Small(46,47,"m/h");
			
			LCD_Wstring_Small(58+10,47,"RS:");//转速
			//LCD_Wstring_Small(72+15,47,Rs);
			LCD_Wstring_Small(82+20,47,"r/m");
			
			//第二排 里程 温度
			LCD_Wstring_Small(0,56,"TRIP:");//里程
			//LCD_Wstring_Small(25+5,56,Trip);
			LCD_Wstring_Small(46,56,"m");
			
			LCD_Wstring_Small(50+18,56,"T :");//温度
			//LCD_Wstring_Small(60+27,56,Temp);
			LCD_Wstring_Small(70+27,56,"C");
			show &= 0xfe;
			//USER_Timer_Init();
			//EA = 1;
		}
		
		if( (show & 0x02) == 0x02)//时分
		{
			LCD_Wstring_Small(80,0,Time);//show 2 time
			show &= 0xfd;
		}
		if( (show & 0x04) == 0x04)
		{
			LCD_Gui_line(35,27,x1,y1,0);//show 3 biao 1 清盘
			//LCD_Gui_Circle(35,27,18);//第一个表盘
			//LCD_Wstring_Small(16,37,"0");//初始刻度
			//LCD_Wstring_Small(51,37,"90");//终止刻度
			y1 = 27 + cos(3.14 / 6 * n1 + 3.14 / 4) * 18;
			x1 = 35 - sin(3.14 / 6 * n1 + 3.14 / 4) * 18;//表1角度算法
			LCD_Gui_line(35,27,x1,y1,1);//show 3 biao 1
			show &= 0xfb;
		}
		if( (show & 0x08) == 0x08)
		{
			LCD_Gui_line(95,27,x2,y2,0);//show 4 biao 2 清盘
			//LCD_Gui_Circle(95,27,18);//第二个表盘
			//LCD_Wstring_Small(76,37,"0");//初始刻度
			//LCD_Wstring_Small(76+35,37,"90");//终止刻度
			y2 = 27 + cos(3.14 / 6 * n2 + 3.14 / 4 ) * 18;
			x2 = 95 - sin(3.14 / 6 * n2 + 3.14 / 4 ) * 18;//表2角度算法
			LCD_Gui_line(95,27,x2,y2,1);//show 4 biao 2
			show &= 0xf7;
		}
		if( (show & 0x10) == 0x10)//时速 MPS
		{
			LCD_Wstring_Small(25,47,Mps);//show 5 MPS
			show &= 0xef;
		}
		if( (show & 0x20) == 0x20)//转速 RS
		{
			LCD_Wstring_Small(72+15,47,Rs);//show 6 RS
			show &= 0xdf;
		}
		if( (show & 0x40) == 0x40)//里程 TRIP
		{
			LCD_Wstring_Small(25,56,Trip);//show 7 TRIP
			show &= 0xbf;
		}
		if( (show & 0x80) == 0x80)//温度 tempture
		{
			LCD_Wstring_Small(60+27,56,Temp);//show 8 TEMP
			show &= 0x7f;
		}
	/***************************************************************/
	
	}
}