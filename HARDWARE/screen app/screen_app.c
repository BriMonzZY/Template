/**
 *    触摸屏响应函数
 *		状态机状态执行函数
 *		屏幕绘制函数
 */


#include "screen_app.h"


/* 变量定义 */
u8 t=0;
u8 i=0;
u16 lastpos[5][2];		/* 最后一次的数据 */

u16 pointer_x1 = 100, pointer_y1 = 7, pointer_x2 = 105, pointer_y2 = 12;  /* ips矩形指针的位置参数 */
int ipsInputNum = 0;


/**
 * @brief    	画水平线
 * @param    	x0,y0:坐标
							len:线长度
							color:颜色
 * @retval   	无
 */
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
    if((x0+len-1)>=lcddev.width)x0=lcddev.width-len-1;	//限制坐标范围
    if(y0>=lcddev.height)y0=lcddev.height-1;			//限制坐标范围
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}


/**
 * @brief    	画实心圆
 * @param    	x0,y0:坐标
							r:半径
							color:颜色
 * @retval   	无
 */
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  


/**
 * @brief   两个数之差的绝对值 
 * @param   x1,x2：需取差值的两个数
 * @retval  返回值：|x1-x2|
 */
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  


/**
 * @brief    画一条粗线
 * @param    (x1,y1),(x2,y2):线条的起始坐标
						  size：线条的粗细程度
						  color：线条的颜色
 * @retval   无
 */
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   





/**
 * @brief	IPS画矩形
 * @param	(x1, y1), (y1, y2)矩形的起始和结尾
 *			color:  矩形的颜色
 *			style:  1实心  0空心
 * @retval	None
 * @author	Brimon
 */
void ipsDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color, u16 style)
{
	u16 i, j;
	/* 实心矩阵 */
	if(style == 1) {
		for(i = y1; i <= y2; i++) {
			for(j = x1; j <= x2; j++) {
				ips114_drawpoint(j, i, color);
			}
		}
	}
	/* 空心矩阵 */
	else {
		for(i = y1; i <= y2; i++) {
			if(i == y1 || i == y2) {
				for(j = x1; j <= x2; j++) {
					ips114_drawpoint(j, i, color);
				}
			}
			else {
				ips114_drawpoint(x1, i, color);
				ips114_drawpoint(x2, i, color);
			}
		}
	}
}










/* **************************************************** 屏幕绘制 ********************************************************************** */


/**
 * @brief:		INIT屏幕绘制函数  仅仅执行一次
 * @param:		none
 * @retval:		none
 * @author:		Brimon
 */
void Draw_INIT_Screen(void)
{
	/* init基本交互界面 */
	#if defined(USE_LCD)
		LCD_Clear(WHITE);
		
		POINT_COLOR=BLUE;
		LCD_DrawRectangle(75, 220, 195, 270);	 	/* MANUAL */
		LCD_DrawRectangle(75, 270,195, 320);		/* AUTO1 */
		LCD_DrawRectangle(75, 320, 195, 370); 	/* AUTO2 */
		LCD_DrawRectangle(75, 370, 195, 420); 	/* FOLLOW */
		POINT_COLOR=CYAN;
		LCD_ShowString(100,20,120,120,24,"WELCOM");
		LCD_ShowString(90,230,100,100,32,"MANUAL");
		LCD_ShowString(95,280,100,100,32,"AUTO1");
		LCD_ShowString(95,330,100,100,32,"AUTO2");	
		LCD_ShowString(95,380,100,100,32,"FOLLOW");
	#endif
	#if defined(USE_IPS)
		ips114_clear(WHITE);
		ips114_showstr(0, 0, (uint8*)"manual");
		ips114_showstr(0, 1, (uint8*)"auto1");
		ips114_showstr(0, 2, (uint8*)"auto2");
		pointer_x1 = 100, pointer_y1 = 7, pointer_x2 = 105, pointer_y2 = 12;
		ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, BLACK, 1);
	#endif
}



/**
 * @brief:		MANUAL屏幕绘制函数  (数字输入界面(测试))
 * @param:		none
 * @retval:		none
 */
void Draw_MAUAL_Screen(void)
{
	#if defined(USE_LCD)
		
	#endif
	#if defined(USE_IPS)
		ips114_clear(WHITE);
	
		ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, WHITE, 1);
		pointer_x1 = 100, pointer_y1 = 7, pointer_x2 = 105, pointer_y2 = 12;
		ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, BLACK, 1);
	
		ips114_showstr(0, 0, (uint8*)"0");
		ips114_showstr(0, 1, (uint8*)"1");
		ips114_showstr(0, 2, (uint8*)"2");
		ips114_showstr(0, 3, (uint8*)"3");
		ips114_showstr(0, 4, (uint8*)"4");
		ips114_showstr(0, 5, (uint8*)"5");
		ips114_showstr(0, 6, (uint8*)"6");
		ips114_showstr(0, 7, (uint8*)"7");
	
		ips114_showstr(130, 0, (uint8*)"8");
		ips114_showstr(130, 1, (uint8*)"9");
		ips114_showstr(130, 2, (uint8*)".");
		ips114_showstr(130, 3, (uint8*)"+");
		ips114_showstr(130, 4, (uint8*)"-");
		ips114_showstr(130, 5, (uint8*)"*");
		ips114_showstr(130, 6, (uint8*)"/");
	#endif
}

void Draw_AUTO1_Screen(void)
{
	#if defined(USE_LCD)
		
	#endif
	#if defined(USE_IPS)
		ips114_clear(WHITE);
		ips114_showstr(0, 0, (uint8*)"auto1");
	#endif
	
}

void Draw_AUTO2_Screen(void)
{
	#if defined(USE_LCD)
		
	#endif
	#if defined(USE_IPS)
		
	#endif
	
}

void Draw_FOLLOW_Screen(void)
{
	#if defined(USE_LCD)
		
	#endif
	#if defined(USE_IPS)
		
	#endif
	
}



/* *************************************************************** 状态动作函数(在while(1)中执行) ************************************************* */


/**
 * @brief:		INIT动作函数	在主界面下循环执行的函数
 * @param:		none
 * @retval:		none
 * @author:		Brimon
 */
void INIT_Action(void)
{
	#if defined(USE_LCD)
		/* 按键按下 */
		if(screen_cnt == 1){
			/* 状态切换 */
			/**************MANUAL****************/
			if(tp_dev.x[t] >= 75 && tp_dev.x[t] <= 195 && tp_dev.y[t] >= 220 && tp_dev.y[t] <= 270){
				BEEP_ONCE();
				Init_To_Manual();  /* 调用状态转移函数 */
			}
			/**************AUTO1****************/
			if(tp_dev.x[t] >= 75 && tp_dev.x[t] <= 195 && tp_dev.y[t] >= 270 && tp_dev.y[t] <= 320){
				BEEP_ONCE();
				Init_To_Auto1();
			}
			/**************AUTO2****************/
			if(tp_dev.x[t] >= 75 && tp_dev.x[t] <= 195 && tp_dev.y[t] >= 320 && tp_dev.y[t] <= 370){
				BEEP_ONCE();
				Init_To_Auto2();
			}
			/**************FOLLOW****************/
			if(tp_dev.x[t] >= 75 && tp_dev.x[t] <= 195 && tp_dev.y[t] >= 370 && tp_dev.y[t] <= 420){
				BEEP_ONCE();
				Init_To_Follow();
			}
		}
	#endif
	
	#if defined(USE_IPS)
		if(keyValue == KEY0_PRES) {  /* 上一项 */
			if(ipsPointerPosition > 1) {
				ipsPointerPosition--;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, WHITE, 1);
				pointer_y1-=16;
				pointer_y2-=16;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, BLACK, 1);
				BEEP_ONCE();
			}
			keyValue = 0;  /* 恢复无按下的状态 */
		}
		else if(keyValue == KEY1_PRES) {  /* 确定 进行状态切换 */
			staSystem = ipsPointerPosition;
			BEEP_ONCE();
			switch(staSystem) {
				case MANUAL: Init_To_Manual(); break;
				case AUTO1: Init_To_Auto1(); break;
				case AUTO2: Init_To_Auto2(); break;
				default: break;
			}
			keyValue = 0;  /* 恢复无按下的状态 */
		}
		else if(keyValue == KEY2_PRES) {  /* 下一项 */
			if(ipsPointerPosition < NUM_OF_MODES-1) {
				ipsPointerPosition++;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, WHITE, 1);
				pointer_y1+=16;
				pointer_y2+=16;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, BLACK, 1);
				BEEP_ONCE();
			}
			keyValue = 0;  /* 恢复无按下的状态 */
		}
	#endif
}


/**
 * @brief:		AUTO1动作函数	在AUTO1状态下循环执行的函数
 * @param:		none
 * @retval:		none
 * @author:		Brimon
 */
void AUTO1_Action(void)
{
	#if defined(USE_LCD)
		
	#endif
	#if defined(USE_IPS)
		if(keyValue == KEY3_PRES) {  /* 返回 状态转移到INIT */
			staSystem = INIT;
			Auto1_To_Init();
			BEEP_ONCE();
			keyValue = 0;  /* 恢复无按下的状态 */
		}
	#endif
}


/**
 * @brief:		MANUAL动作函数	在MANUAL状态下循环执行的函数
 * @param:		none
 * @retval:		none
 * @author:		Brimon
 */
void MANUAL_Action(void)
{
	#if defined(USE_LCD)
		
	#endif
	
	#if defined(USE_IPS)
		if(keyValue == KEY0_PRES) {  /* 上一项 */
			if(ipsPointerPosition > 1 && ipsPointerPosition < 9) {
				ipsPointerPosition--;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, WHITE, 1);
				pointer_y1-=16;
				pointer_y2-=16;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, BLACK, 1);
				BEEP_ONCE();
			}
			else if(ipsPointerPosition == 9) {
				ipsPointerPosition--;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, WHITE, 1);  /* 清除之前的指针显示 */
				pointer_x1 = 100, pointer_y1 = 7+16*7, pointer_x2 = 105, pointer_y2 = 12+16*7;  /* 重新确定位置 */
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, BLACK, 1);  /* 绘制 */
			}
			else if(ipsPointerPosition > 9 && ipsPointerPosition <= 15) {
				ipsPointerPosition--;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, WHITE, 1);
				pointer_y1-=16;
				pointer_y2-=16;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, BLACK, 1);
			}
			keyValue = 0;  /* 恢复无按下的状态 */
		}
		
		
		else if(keyValue == KEY1_PRES) {  /* 确定  */
			switch(ipsPointerPosition) {
				case 1: ipsInputNum*=10; ipsInputNum+=0; break;
				case 2: ipsInputNum*=10; ipsInputNum+=1; break;
				case 3: ipsInputNum*=10; ipsInputNum+=2; break;
				case 4: ipsInputNum*=10; ipsInputNum+=3; break;
				case 5: ipsInputNum*=10; ipsInputNum+=4; break;
				case 6: ipsInputNum*=10; ipsInputNum+=5; break;
				case 7: ipsInputNum*=10; ipsInputNum+=6; break;
				case 8: ipsInputNum*=10; ipsInputNum+=7; break;
				case 9: ipsInputNum*=10; ipsInputNum+=8; break;
				case 10: ipsInputNum*=10; ipsInputNum+=9; break;
				case 11: break;
				case 12: break;
				case 13: ipsInputNum*=-1; break;
				case 14: break;
				case 15: break;
				default: break;
			}
			
			ips114_showint16(130, 7, ipsInputNum);
			
			
			BEEP_ONCE();
		}
		
		
		else if(keyValue == KEY2_PRES) {  /* 下一项 */
			/* 指针在第一列 */
			if(ipsPointerPosition < 8) {
				ipsPointerPosition++;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, WHITE, 1);
				pointer_y1+=16;
				pointer_y2+=16;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, BLACK, 1);
				BEEP_ONCE();
			}
			else if(ipsPointerPosition == 8) {  /* 第二列第一个 进行一下位置变换 */
				ipsPointerPosition++;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, WHITE, 1);  /* 清除之前的指针显示 */
				pointer_x1 = 230, pointer_y1 = 7, pointer_x2 = 235, pointer_y2 = 12;  /* 重新确定位置 */
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, BLACK, 1);  /* 绘制 */
			}
			/* 指针在第二列 */
			else if(ipsPointerPosition > 8 && ipsPointerPosition < 15) {
				ipsPointerPosition++;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, WHITE, 1);
				pointer_y1+=16;
				pointer_y2+=16;
				ipsDrawRectangle(pointer_x1, pointer_y1, pointer_x2, pointer_y2, BLACK, 1);
			}
			keyValue = 0;  /* 恢复无按下的状态 */
		}
		
		
		else if(keyValue == KEY3_PRES) {  /* 返回 状态转移到INIT */
			staSystem = INIT;
			Manual_To_Init();
			BEEP_ONCE();
			keyValue = 0;  /* 恢复无按下的状态 */
		}
		
		
		
		
	#endif
}


void Button_App(void)
{
	printf("[Log] screen_app : location : %d %d \n", tp_dev.x[t], tp_dev.y[t]);  /* 输出坐标 */
}
