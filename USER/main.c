/**
 * @project	
 * @author 五月花队
 * @members	潘璇岳 张中尧 孙昕
 * @brief 电赛
 * @version 0.1
 * @date 2021-8-4
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#define _MAIN_C
#include "main.h"

void globalInit(void);
/* 全局变量定义 */

uint8_t temp_usart3;					/* uart3接收缓冲区 */
enum estaSystem staSystem = INIT;		/* 设置初始状态staSystem为INIT */
u16 screen_cnt = 0;						/* 触摸屏计数 */
u8 keyValue = 0;						/* 按键按下的键值 */
u16 ipsPointerPosition = 1;				/* ips屏幕下指针的位置   从1开始 */


/* ****************************************主函数*********************************** */

/**
 * @brief:		主函数
 * @param:		none
 * @retval:		none
 */
int main(void)
{
	/* 变量定义区域 */
	
	/* 初始化区域 */
	globalInit();											/* 全局初始化 */
	
/********************************************while(1)**********************************************/
	while(1) {
		/* 检测按键 */
		keyValue = KEY_Scan(0);
		
		/* 触摸屏处理部分 */
		#if defined(USE_LCD)
			tp_dev.scan(0);									/* 扫描触摸屏 */
			if(tp_dev.sta & TP_PRES_DOWN){					/* 触摸屏被按下 */
				if(tp_dev.sta & TP_PRES_DOWN){				/* 触摸屏被按下 */
					Button_App();							/* 触摸屏按下动作 函数中会打印坐标 */
					screen_cnt++;
					delay_ms(50);
				}
			}
			if(!(tp_dev.sta&TP_PRES_DOWN)) {   	/* 触摸屏被松开 */ 
				if(!(tp_dev.sta&TP_PRES_DOWN)){		/* 触摸屏被松开 */
					screen_cnt = 0;
				}
			}
		#endif
		
		/* 状态机逻辑判断 */
		if(staSystem == INIT){
			INIT_Action();
		}
		if(staSystem == AUTO1){
			AUTO1_Action();
		}
		if(staSystem == MANUAL){
			MANUAL_Action();
		}
		
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
		
		
	}  /* end of while(1) */
}


/**
 * @brief:		全局初始化
 * @param:		none
 * @retval:		none
 */
void globalInit(void)
{
	HAL_Init();											/* 初始化HAL库 */
	Stm32_Clock_Init(360,25,2,8);   					/* 设置时钟,180MHz */
	delay_init(180);                					/* 初始化延时函数 180MHz */
	MX_GPIO_Init();										/* 初始化GPIO */
	MX_DMA_Init();										/* 初始化DMA */
	MX_USART1_UART_Init();								/* 初始化uart1 */
	MX_USART3_UART_Init();								/* 初始化uart3 */
	MX_TIM5_Init();										/* 初始化定时器TIM5 */
	StartUartRxDMA();  									/* 使能uartDMA(uart3) */
	HAL_UART_Receive_IT(&huart3, &temp_usart3, 1);  	/* 打开uart中断 */
	LED_Init();                     					/* 初始化LED */
	KEY_Init();                     					/* 初始化按键 */
	HAL_TIM_Base_Start_IT(&htim5);   					/* 定时器5使能 */
	BEEP_Init();										/* 初始化蜂鸣器 */
	
	
	/* 初始化LCD */
	#if defined(USE_LCD) && defined(NOT_USE_STEER) && defined(NOT_USE_STEER)
		SDRAM_Init();                   					/* 初始化SDRAM*/
		LCD_Init();                     					/* 初始化LCD */
		tp_dev.init();				    					/* 触摸屏初始化 */ 
		POINT_COLOR=BLUE;									/* 笔触颜色 */
		Draw_INIT_Screen();									/* 初始化屏幕为Init界面 */
	#endif
	
	
	/* 初始化IPS */
	#if defined(USE_IPS) || (defined(USE_LCD) && defined(USE_STEER))
		ips114_init();
		ips114_clear(WHITE);
		ips114_display_chinese(0,1*16,16,chinese_test[8],3,RED);
		ips114_display_chinese(0,3*16,16,chinese_test[24],3,BLUE);
		ips114_display_chinese(5*16,3*16,16,chinese_test[14],3,BLUE);
		ips114_display_chinese(10*16,3*16,16,chinese_test[20],2,BLUE);
		ips114_showuint16(0,5,2021);
		HAL_Delay(600);
		ips114_clear(WHITE);
		Draw_INIT_Screen();
	#endif
	
	
	/* 初始化舵机 */
	#if defined(USE_STEER)
		/* 舵机控制PWM初始化 */
		MX_TIM12_Init();
		MX_TIM13_Init();
		MX_TIM14_Init();
		HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim13,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim14,TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, 75);
		__HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, 75);
		__HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, 75);
		__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 75);
	#endif
	
	
	/* 初始化电机 */
	#if defined(USE_MORTOR)
		MX_TIM8_Init();
		MX_TIM9_Init();
		MX_TIM11_Init();
		HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim11,TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 0);
		__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, 0);
	#endif
	
	
	/* 初始化编码器 */
	#if defined(USE_ENCODER)
		MX_TIM1_Init();
		MX_TIM2_Init();
		MX_TIM3_Init();
		MX_TIM4_Init();
		HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);  /* 启动编码器模式 */
		HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
		HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
		HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
	#endif
	
	
	/* 初始化激光测距 */
	#if defined(USE_LASER_RANGING)
		
	#endif
	
	
	/* 初始化陀螺仪 */
	#if defined(USE_ICM20602)
		icm20602_init_simspi();
	#endif
	
	printf("[info] main.c : init finished\n\n");	
	BEEP_ONCE();
}


void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}
