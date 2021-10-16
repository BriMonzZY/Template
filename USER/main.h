#ifndef __MAIN_H
#define __MAIN_H



/**********************************全局状态*******************************************/
/* LCD IPS */#define USE_LCD				/* USE_LCD				USE_IPS		LCD和IPS不能共存显示*/
/* 舵机 */#define NOT_USE_STEER		/* NOT_USE_STEER		USE_STEER	是否使用舵机 舵机和LCD不能共存*/
/* 电机 */#define NOT_USE_MORTOR		/* NOT_USE_MORTOR		USE_MORTOR */
/* 编码器 */#define USE_ENCODER		/* NOT_USE_ENCODER		USE_ENCODER */
/* 无线串口 */#define NOT_USE_WIRELESS	/* NOT_USE_WIRELESS		USE_WIRELESS */
/* 陀螺仪 */#define	NOT_USE_ICM20602	/* USE_ICM20602_1		USE_ICM20602_2 */
/* 激光测距 */#define NOT_USE_LASER_RANGING/* NOT_USE_LASER_RANGING USE_LASER_RANGING */
/* 蜂鸣器开关 */#define BEEP_ON   /* BEEP_ON BEEP_OFF */


/* 一共有几个模式(包括INIT模式) */#define NUM_OF_MODES 4



/* --------------------------------Includes ---------------------------------*/
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "touch.h"
#include "screen_app.h"
#include "SEEKFREE_IPS114_SPI.h"
#include "main.h"
#include "dma.h"
#include "uart_dma.h"
#include "status_transmit.h"
#include "beep.h"
#include "motor.h"
#include <stdio.h>
#include <math.h>


/* 状态机状态 */
enum estaSystem {  /* 枚举系统状态   系统状态的初始化顺序要和ips显示屏上显示的顺序一致 */
	INIT, MANUAL, AUTO1, AUTO2, FOLLOW
};

/* global extern */
#ifndef _MAIN_C
	extern enum estaSystem staSystem;//staSystem当前系统状态
	extern u16 screen_cnt;
	extern u8 keyValue;
	extern u16 ipsPointerPosition;
	extern u16 screen_cnt;
#endif




#define IPS114_BL_Pin GPIO_PIN_13
#define IPS114_BL_GPIO_Port GPIOE
#define IPS114_CS_Pin GPIO_PIN_14
#define IPS114_CS_GPIO_Port GPIOE
#define IPS114_DC_Pin GPIO_PIN_15
#define IPS114_DC_GPIO_Port GPIOE
#define IPS114_REST_Pin GPIO_PIN_8
#define IPS114_REST_GPIO_Port GPIOD
#define IPS114_SDA_Pin GPIO_PIN_9
#define IPS114_SDA_GPIO_Port GPIOD
#define IPS114_SCL_Pin GPIO_PIN_10
#define IPS114_SCL_GPIO_Port GPIOD


void Error_Handler(void);



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
