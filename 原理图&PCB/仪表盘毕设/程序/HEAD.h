/**********************��ͷ�ļ�*********************************
*  ����:11.0592MHZ
******************************************************************/
#ifndef __HEAD
#define __HEAD 1
/**********************�����ļ�**********************************************/
#include <reg52.h>
/************************************�궨��************************************/
#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long
/************************************λ����************************************/

/**********************************��������***********************************/
void USER_Timer_Init(void);

/*********************************ȫ�ֱ�������***********************************/  
extern uchar show;
extern uchar speed;            
extern uchar mcs;          
extern uint Data_Show;     //��������ʾ
extern uint user_time;
extern double trip_data;
extern uint mps_data;
//extern uint pwm_count;
#endif