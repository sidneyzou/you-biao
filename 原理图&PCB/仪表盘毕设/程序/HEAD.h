/**********************主头文件*********************************
*  晶振:11.0592MHZ
******************************************************************/
#ifndef __HEAD
#define __HEAD 1
/**********************包含文件**********************************************/
#include <reg52.h>
/************************************宏定义************************************/
#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long
/************************************位定义************************************/

/**********************************函数定义***********************************/
void USER_Timer_Init(void);

/*********************************全局变量定义***********************************/  
extern uchar show;
extern uchar speed;            
extern uchar mcs;          
extern uint Data_Show;     //编码器显示
extern uint user_time;
extern double trip_data;
extern uint mps_data;
//extern uint pwm_count;
#endif