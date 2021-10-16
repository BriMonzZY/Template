#ifndef __MAIN_H
#define __MAIN_H



/**********************************ȫ��״̬*******************************************/
/* LCD IPS */#define USE_LCD				/* USE_LCD				USE_IPS		LCD��IPS���ܹ�����ʾ*/
/* ��� */#define NOT_USE_STEER		/* NOT_USE_STEER		USE_STEER	�Ƿ�ʹ�ö�� �����LCD���ܹ���*/
/* ��� */#define NOT_USE_MORTOR		/* NOT_USE_MORTOR		USE_MORTOR */
/* ������ */#define USE_ENCODER		/* NOT_USE_ENCODER		USE_ENCODER */
/* ���ߴ��� */#define NOT_USE_WIRELESS	/* NOT_USE_WIRELESS		USE_WIRELESS */
/* ������ */#define	NOT_USE_ICM20602	/* USE_ICM20602_1		USE_ICM20602_2 */
/* ������ */#define NOT_USE_LASER_RANGING/* NOT_USE_LASER_RANGING USE_LASER_RANGING */
/* ���������� */#define BEEP_ON   /* BEEP_ON BEEP_OFF */


/* һ���м���ģʽ(����INITģʽ) */#define NUM_OF_MODES 4



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


/* ״̬��״̬ */
enum estaSystem {  /* ö��ϵͳ״̬   ϵͳ״̬�ĳ�ʼ��˳��Ҫ��ips��ʾ������ʾ��˳��һ�� */
	INIT, MANUAL, AUTO1, AUTO2, FOLLOW
};

/* global extern */
#ifndef _MAIN_C
	extern enum estaSystem staSystem;//staSystem��ǰϵͳ״̬
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
