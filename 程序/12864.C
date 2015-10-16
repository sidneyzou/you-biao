/**********************12864函数文件*********************************
*  晶振:11.0592MHZ
******************************************************************/

/**********************包含文件*******************/
#include <12864.h>
#include "english_5x8_pixel.h"
/*********************************全局变初始化***********************************/ 
uchar code DCB2HEX_TAB[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

/******************************************************************************/
/* 函数名称  : LCD_Delay                                                      */
/* 函数描述  : 延时函数                                                       */
/* 输入参数  : uint xms                                                       */
/* 参数描述  : 延迟                                                           */
/* 返回值    : 无                                                             */
/******************************************************************************/	
void LCD_Delay(uint ms)
{
    while(ms--)
	{
      uint i;
		 for(i=0;i<250;i++)  
	   {
	    _nop_();			   
			_nop_();
			_nop_();
			_nop_();
	   }
	}
}		
/******************************************************************************/
/* 函数名称  : LCD_Busy                                                       */
/* 函数描述  : LCD查忙                                                        */
/* 输入参数  : void                                                           */
/* 参数描述  : 无                                                             */
/* 返回值    : lcd_busy为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据      */
/******************************************************************************/
bit LCD_Busy(void)
{                          
	bit result;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_EN = 1;
	LCD_DELAYNOP;
	result = (bit)(LCD_DATA&0x80);
	LCD_EN = 0;
	return(result); 
}
/******************************************************************************/
/* 函数名称  : LCD_Wcmd                                                       */
/* 函数描述  : 写指令数据到LCD  RS=L，RW=L，E=高脉冲，D0-D7=指令码            */
/* 输入参数  : uchar cmd                                                      */
/* 参数描述  : 输入命令                                                       */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Wcmd(uchar cmd)
{                          
   while(LCD_Busy());
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    LCD_DELAYNOP;
    LCD_DATA = cmd;
    LCD_DELAYNOP;
    LCD_EN = 1;
    LCD_DELAYNOP;
    LCD_EN = 0;  
}
/******************************************************************************/
/* 函数名称  : LCD_Wdat                                                       */
/* 函数描述  : 写显示数据到LCD  RS=H，RW=L，E=高脉冲，D0-D7=数据              */
/* 输入参数  : uchar dat                                                      */
/* 参数描述  : 输入数据                                                       */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Wdat(uchar dat)
{                          
   while(LCD_Busy());
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
		LCD_DELAYNOP;
    LCD_DATA = dat;
    LCD_DELAYNOP;
    LCD_EN = 1;
    LCD_DELAYNOP;
    LCD_EN = 0; 
}
/******************************************************************************/
/* 函数名称  : LCD_Rdat                                                       */
/* 函数描述  : LCD读数据                                                      */
/* 输入参数  : void                                                           */
/* 参数描述  : 无                                                             */
/* 返回值    : 无                                                             */
/******************************************************************************/
uchar LCD_Rdat(void)
{
	uchar temp;
	LCD_DATA = 0XFF; 
	LCD_RS = 1; 
	LCD_RW = 1; 
	LCD_EN = 1; 
	LCD_Delay_S(1);
	temp = LCD_DATA;
	LCD_EN = 0;
	return temp;
}
/******************************************************************************/
/* 函数名称  : LCD_Init                                                       */
/* 函数描述  : LCD初始化                                                      */
/* 输入参数  : void                                                           */
/* 参数描述  : 无                                                             */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Init(void)
{ 

    LCD_PSB = 1;         //并口方式
    
    LCD_Wcmd(0x34);      //扩充指令操作
    LCD_Delay(5);
    LCD_Wcmd(0x30);      //基本指令操作
    LCD_Delay(5);
    LCD_Wcmd(0x0C);      //显示开，关光标
    LCD_Delay(5);
    LCD_Wcmd(0x01);      //清除LCD的显示内容
    LCD_Delay(5);
}
/******************************************************************************/
/* 函数名称  : LCD_Pos                                                        */
/* 函数描述  : 显示位置                                                       */
/* 输入参数  : uchar x ,uchar y                                               */
/* 参数描述  : 坐标x y                                                        */
/* 返回值    : 无                                                             */
/******************************************************************************
void LCD_Pos(uchar X,uchar Y)
{                          
   uchar  pos;
   if (X==0)
     {X=0x80;}
   else if (X==1)
     {X=0x90;}
   else if (X==2)
     {X=0x88;}
   else if (X==3)
     {X=0x98;}
   pos = X+Y ;  
   LCD_Wcmd(pos);     //显示地址
}
/******************************************************************************/
/* 函数名称  : LCD_Wstring                                                    */
/* 函数描述  : 显示字符串                                                     */
/* 参数描述  : 坐标x y 字符串指针Alphabet                                     */
/* 返回值    : 无                                                             */
/******************************************************************************
void LCD_Wstring(uchar x,uchar y,uchar *Alphabet)
{
	uchar i=0;
	LCD_Pos(x,y);
	while(Alphabet[i]!='\0')
	{
		LCD_Wdat(Alphabet[i]);
		i++;
	}
}

/******************************************************************************/
/* 函数名称  : LCD_Gui_Clear                                                  */
/* 函数描述  : 绘图清屏                                                       */
/* 参数描述  : 无                                                             */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Gui_Clear()
{
	uchar i , j , k;
	LCD_Wcmd(EXTEND_SET);//扩展指令集 8位数据传输
	LCD_Wcmd(DRAW_OFF);//绘图显示关闭
	for(i = 0; i < 2; i ++)//分上下两屏写
	{
		for(j = 0; j < 32; j ++)
		{
			LCD_Wcmd(0x80 + j);//写Y轴
			LCD_Delay_S(1);
			if(i == 0) //写X轴
			{
				LCD_Wcmd(0x80);
				LCD_Delay_S(1);
			}
			else //写下半屏
			{
				LCD_Wcmd(0x88);
				LCD_Delay_S(1);
			}
			for(k = 0; k < 16; k ++)//写一整行数据
			{
				LCD_Wdat(0x00);//写高字节
				LCD_Wdat(0x00);//写低字节
				LCD_Delay_S(1);
			}
		}
	}
	LCD_Wcmd(DRAW_ON);//打开绘图显示
	LCD_Wcmd(BASIC_SET);//打开基本指令集
}
/******************************************************************************/
/* 函数名称  : LCD_Gui_Point                                                  */
/* 函数描述  : 黑白打点                                                       */
/* 参数描述  : 参数:color=1,该点填充1;color=0,该点填充0                       */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Gui_Point(uchar x,uchar y,uchar color)
{
	uchar x_Dyte,x_byte; //定义列地址字节位，和在字节中哪一位
	uchar y_Dyte,y_byte; //定义为上下两个屏（取值0，1），行地址（0-31）
	uchar GDRAM_hbit,GDRAM_lbit;
	LCD_Wcmd(0x36); //扩展指令集
	/*X,Y坐标互换,即普通的X，Y坐标*/
	x_Dyte=x/16; //计算在16个字节中哪一位
	x_byte=x&0x0f; //计算在该字节中的哪一位
	y_Dyte=y/32; //0为上半屏 1为下半屏
	y_byte=y&0x1f; //计算在0-31当中的哪一行
	LCD_Wcmd(0x80+y_byte); //设定行地址Y
	LCD_Wcmd(0x80+x_Dyte+8*y_Dyte); // 设定列地址X 选上下屏
	LCD_Rdat();// 读取当前数据
	GDRAM_hbit= LCD_Rdat(); //读取当前显示高8位
	GDRAM_lbit= LCD_Rdat(); //读取当前显示低8位
	LCD_Delay_S(1);
	LCD_Wcmd(0x80+y_byte); //设定行地址Y
	LCD_Wcmd(0x80+x_Dyte+8*y_Dyte);// 设定列地址X 选上下屏
	LCD_Delay_S(1);
	if(x_byte<8) //判断高8位 还是低8位
	{
		if(color==1)
		{
			LCD_Wdat(GDRAM_hbit|(0x01<<(7-x_byte))); //置位GDRAM区高8位数据中相应的点
		}
		else
			LCD_Wdat(GDRAM_hbit&(~(0x01<<(7-x_byte)))); //清除GDRAM区高8位数据中相应的点
			LCD_Wdat(GDRAM_lbit); //显示GDRAM区低8位数据中相应的点
		}
	else
	{
		LCD_Wdat(GDRAM_hbit);
		if(color==1)
			LCD_Wdat(GDRAM_lbit|(0x01<<(15-x_byte))); //置位GDRAM区高8位数据中相应的点
		else
			LCD_Wdat(GDRAM_lbit&(~(0x01<<(15-x_byte))));//清除GDRAM区高8位数据中相应的点
	}
	LCD_Wcmd(0x30); //恢复到基本指令集
}


/******************************************************************************/
/* 函数名称  : LCD_Gui_Point_A                                                */
/* 函数描述  : 任意打点                                                       */
/* 参数描述  :                                                                */
/* 返回值    : 无                                                             */
/******************************************************************************
void LCD_Gui_Point_A(uchar x,uchar y)
{
	uchar x_byte,x_bit;//确定坐标在哪一字节哪一位
	uchar y_ping , y_bit;//确定坐标在哪一屏哪一行
	uchar tmph , tmpl;//定义两个临时变量 用于存放读出来的数据
	LCD_Wcmd(EXTEND_SET);//扩展指令集
	LCD_Wcmd(DRAW_OFF);//绘图显示关
	x_byte = x / 16;//在哪一字节
	x_bit = x % 16;//算出在哪一位
	y_ping = y / 32;//确定上下屏
	y_bit = y % 32;//确定哪一行
	LCD_Wcmd(0X80 + y_bit);//先写垂直地址
	LCD_Wcmd(0x80 + x_byte + 8 * y_ping);//水平起始坐标 确定上下屏
	LCD_Rdat();//预读取数据
	tmph = LCD_Rdat();//读取当前显示高8位数据
	tmpl = LCD_Rdat();//读取当前显示低8位数据
	LCD_Delay_S(1);
	LCD_Wcmd(0x80 + y_bit);//读操作会改变AC 重设一次
	LCD_Wcmd(0x80 + x_byte + 8 * y_ping);
	LCD_Delay_S(1);
	if(x_bit < 8)
	{
		LCD_Wdat(tmph | (0x01 << (7 - x_bit)));//写高字节
		LCD_Wdat(tmpl);//原低位数据送回
	}
	else
	{
		LCD_Wdat(tmph);//原高位数据送回
		LCD_Wdat(tmpl | (0x01 << (15 - x_bit)));
	}
	LCD_Wcmd(DRAW_ON); //打开绘图显示
	LCD_Wcmd(BASIC_SET);//回到基本指令集
	
}
/******************************************************************************/
/* 函数名称  : LCD_Gui_Hline                                                  */
/* 函数描述  : 画水平线                                                       */
/* 参数描述  : x0 起始坐标,x1 终点坐标,y 垂直坐标 color                       */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Gui_Hline(uchar x0, uchar x1, uchar y , uchar color)
{
	uchar bak;
	if(x0 > x1)
	{
		bak = x1;
		x1 = x0;
		x0 = bak;
	}
	do
	{
		LCD_Gui_Point(x0 , y , color);
		x0 ++;
	}while(x1 >= x0);
}

/******************************************************************************/
/* 函数名称  : LCD_Gui_Rline                                                  */
/* 函数描述  : 画竖直线                                                       */
/* 参数描述  : y0起始坐标,y1 终点坐标,x 水平坐标  color                       */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Gui_Rline(uchar x, uchar y0, uchar y1 , uchar color)
{
	uchar bak;
	if(y0 > y1)
	{
		bak = y1;
		y1 = y0;
		y0 = bak;
	}
	do
	{
		LCD_Gui_Point(x , y0 , color);
		y0 ++;
	}while(y1 >= y0);
}
/******************************************************************************/
/* 函数名称  : LCD_Gui_Linewith                                               */
/* 函数描述  : 任意连线                                                       */
/* 参数描述  : x0,y0 起始坐标,x1,y1 终点坐标,with 线宽,color黑白              */
/* 返回值    : 无                                                             */
/******************************************************************************
void LCD_Gui_Linewith(uchar x0 , uchar y0 , uchar x1 , uchar y1 , uchar with , uchar color)
{
	char dx; 
	char dy; 
	char dx_sym; 
	char dy_sym; 
	char dx_x2; 
	char dy_x2;
	char di;
	char wx, wy; 
	char draw_a, draw_b;
	if(with==0) return;
	if(with>50) with = 50;
	dx = x1-x0;
	dy = y1-y0;
	wx = with/2;
	wy = with-wx-1;
	if(dx>0) 
	{
		dx_sym = 1; 
	}
	else
	{
		if(dx<0)
		{
			dx_sym = -1; 
		}
		else
		{
			wx = x0-wx;
			if(wx<0) wx = 0;
			wy = x0+wy;
			while(1)
			{
				x0 = wx;
				LCD_Gui_Rline(x0, y0, y1, color);
				if(wx>=wy) break;
				wx++;
			}
			return;
		}
	}
	if(dy>0) 
	{
		dy_sym = 1; 
	}
	else
	{
		if(dy<0)
		{
			dy_sym = -1;
		}
		else
		{
			wx = y0-wx;
			if(wx<0) wx = 0;
			wy = y0+wy;
			while(1)
			{
				y0 = wx;
				LCD_Gui_Hline(x0, x1, y1, color);
				if(wx>=wy) break;
				wx++;
			}
			return;
		}
	}
	dx = dx_sym * dx;
	dy = dy_sym * dy;
	dx_x2 = dx*2;
	dy_x2 = dy*2;
	if(dx>=dy) 
	{
		di = dy_x2 - dx;
		while(x0!=x1)
		{
			draw_a = y0-wx;
			if(draw_a<0) draw_a = 0;
			draw_b = y0+wy;
			LCD_Gui_Rline(x0, draw_a, draw_b, color);
			x0 += dx_sym;
			if(di<0)
			{
				di += dy_x2;
			}
			else
			{
				di += dy_x2 - dx_x2;
				y0 += dy_sym;
			}
		}
		draw_a = y0-wx;
		if(draw_a<0) draw_a = 0;
		draw_b = y0+wy;
		LCD_Gui_Rline(x0, draw_a, draw_b, color);
	}
	else 
	{
		di = dx_x2 - dy;
		while(y0!=y1)
		{
			draw_a = x0-wx;
			if(draw_a<0) draw_a = 0;
			draw_b = x0+wy;
			LCD_Gui_Hline(draw_a, y0, draw_b, color);
			y0 += dy_sym;
			if(di<0)
			{
				di += dx_x2;
			}
			else
			{
				di += dx_x2 - dy_x2;
				x0 += dx_sym;
			}
		}
		draw_a = x0-wx;
		if(draw_a<0) draw_a = 0;
		draw_b = x0+wy;
		LCD_Gui_Hline(draw_a, y0, draw_b, color);
	}
}
/******************************************************************************/
/* 函数名称  : LCD_Gui_line                                                   */
/* 函数描述  : 任意连线                                                       */
/* 参数描述  : x0,y0 起始坐标,x1,y1 终点坐标,color黑白                        */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Gui_line(uchar x0 , uchar y0 , uchar x1 , uchar y1 , uchar color)
{
	char dx;
	char dy;
	char dx_sym;
	char dy_sym;
	char dx_x2;
	char dy_x2;
	char di; 
	if(x0 == x1)
	{
		LCD_Gui_Rline(x0 , y0 , y1 , color);
		return;
	}
	if(y0 == y1)
	{
		LCD_Gui_Hline(x0 , x1 , y0 , color);
		return;
	}
	dx = x1 - x0;
	dy = y1 - y0;
	if(dx > 0)
	dx_sym = 1;
	else
	{
		if(dx < 0)
			dx_sym = -1;
		else
		{
			LCD_Gui_Rline(x0 , y0 , y1 , color);
			return;
		}
	}
	if(dy > 0)
		dy_sym = 1;
	else
	{
		if(dy < 0)
			dy_sym = -1;
		else
		{
			LCD_Gui_Hline(x0 , x1 , y0 , color);
			return;
		}
	}
	
	dx = dx_sym * dx;
	dy = dy_sym * dy;
	dx_x2 = dx * 1;
	dy_x2 = dy * 1;
	if(dx >= dy)
	{
		di = dy_x2 - dx;
		while(x0 != x1)
		{
			LCD_Gui_Point(x0,y0,color);
			x0 +=dx_sym;
			if(di < 0)
				di += dy_x2;
			else
			{
				di += dy_x2 - dx_x2;
				y0 += dy_sym;
			}
		}
		LCD_Gui_Point(x0, y0,color);
	}
	else 
	{
		di = dx_x2 - dy;
		while(y0 != y1)
		{
			LCD_Gui_Point(x0, y0,color);
			y0 += dy_sym;
			if(di < 0)
				di += dx_x2;
			else
			{
				di += dx_x2 - dy_x2;
				x0 += dx_sym;
			}
		}
			LCD_Gui_Point(x0, y0,color);
	}
}
/******************************************************************************/
/* 函数名称  : LCD_Gui_Circle                                                 */
/* 函数描述  : 画圆                                                           */
/* 参数描述  : x0,y0 圆心坐标,r 圆半径                                        */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Gui_Circle(uchar x0 , uchar y0 , uchar r)
{
	char a , b;
	char di;
	if(r > 31 || r == 0)
		return;
	a = 0;
	b = r;
	di = 3 - 2 * r;
	while(a <= b)
	{
		LCD_Gui_Point( x0 - b , y0 - a , 1);//3
		LCD_Gui_Point( x0 + b , y0 - a , 1); //0
		LCD_Gui_Point( x0 - a , y0 + b , 1); //1
		LCD_Gui_Point( x0 - b , y0 - a , 1); //7
		LCD_Gui_Point( x0 - a , y0 - b , 1); //2
		LCD_Gui_Point( x0 + b , y0 + a , 1); //4
		LCD_Gui_Point( x0 + a , y0 - b , 1); //5
		LCD_Gui_Point( x0 + a , y0 + b , 1); //6
		LCD_Gui_Point( x0 - b , y0 + a , 1);
		a ++;
	
	/***使用bresenham 算法画圆********/
		if(di < 0)
			di += 4 * a + 6;
		else
		{
			di += 10 + 4 * (a - b);
			b --;
		}
		LCD_Gui_Point( x0 + a , y0 + b , 1);
	}
	/*********************************/
} 

/******************************************************************************/
/* 函数名称  : LCD_Gui_Put_Area                                               */
/* 函数描述  : 自定义区域绘图                                                 */
/* 参数描述  : // x,y 指定显示区域的起始坐标                                  */
/*						 // dat 要输出的显示的图形或者汉字点阵数组                      */
/*						 // hno 要显示的区域的长度                                      */
/*						 // lno 要显示的区域的宽度                                      */
/*						 // flag 反显标志  1反显                                        */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Gui_Put_Area(uchar x,uchar y,uchar *dat,uchar hno,uchar lno,uchar flag)
{
	uchar i;
	for(i=0;i<lno;i++)
	{
		LCD_Gui_Loadline(x,y,dat,hno,flag);
		y++; 
		dat += (hno>>3); 
		if((hno&0x07)!=0) 
			dat++; 
	}
}

/******************************************************************************/
/* 函数名称  : LCD_Gui_Loadline                                               */
/* 函数描述  : 自定义区域绘图                                                 */
/* 参数描述  : // x,y 指定显示区域的起始坐标                                  */
/*						 // *dat 要输出的显示的图形或者汉字点阵数组                     */
/*						 // no 此行显示个数                                             */                                
/*						 // flag 反显标志  1反显                                        */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Gui_Loadline(uchar x,uchar y,uchar *dat,uchar no,uchar flag)
{
	uchar bit_dat;
	uchar i;
	if(x>127) return;
	if(y>63) return;
	for(i=0; i<no; i++)
	{
		if( (i%8)==0 ) bit_dat = *dat++;
		if( (bit_dat&DCB2HEX_TAB[i&0x07])==0 )
		{
			if(flag==0) 
				LCD_Gui_Point(x,y,0); 
			else
				LCD_Gui_Point(x,y,1); 
		}
		else
		{
			if(flag==0)
				LCD_Gui_Point(x,y,1);
			else
				LCD_Gui_Point(x,y,0);
		}
		if( (++x)>127) return;
	}
}
/******************************************************************************/
/* 函数名称  : LCD_Gui_Put_Area                                               */
/* 函数描述  : 5*8小字输出                                                    */
/* 参数描述  : // x,y 指定显示区域的起始坐标                                  */
/*						 // dat 要输出的显示的ASII码                                    */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Wchar_Small(uchar x,uchar y,uchar dat)
{
	switch(dat)
	{
		case 0x20:LCD_Gui_Put_Area(x,y,font5x8[0],5,8,0);break;
		case 0x2f:LCD_Gui_Put_Area(x,y,font5x8[1],5,8,0);break;
		default: LCD_Gui_Put_Area(x,y,font5x8[dat - 0x2e],5,8,0);break;
	}
}
/******************************************************************************/
/* 函数名称  : LCD_Wstring_Small                                              */
/* 函数描述  : 5*8小字字符串输出                                              */
/* 参数描述  : // x,y 指定显示区域的起始坐标                                  */
/*						 // *dat 要输出的显示的字符串                                   */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Wstring_Small(uchar x,uchar y,uchar *dat)
{
	uchar i;
	for(i = 0; dat[i] != '\0'; i++)
	{
		LCD_Wchar_Small(x+i*5,y,dat[i]);
	}
}
/******************************************************************************/
/* 函数名称  : LCD_Delay_S                                                    */
/* 函数描述  : 短延时                                                         */
/* 参数描述  : 延时时间                                                       */
/* 返回值    : 无                                                             */
/******************************************************************************/
void LCD_Delay_S(uint k)
{
	uint i;
	uint j;
	for(i = 0; i < k ;i++)
		for(j = 0; j < 10 ;j++);
}

