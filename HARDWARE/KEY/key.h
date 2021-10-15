#ifndef _KEY_H
#define _KEY_H

#include "main.h"

//����ķ�ʽ��ͨ��λ��������ʽ��ȡIO
//#define KEY0        PHin(3) //KEY0����PH3
//#define KEY1        PHin(2) //KEY1����PH2
//#define WK_UP       PAin(0) //WKUP����PA0


//����ķ�ʽ��ͨ��ֱ�Ӳ���HAL�⺯����ʽ��ȡIO
#define KEY0        HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)  //KEY0����PA2
#define KEY1        HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_2)  //KEY1����PH2
#define KEY2        HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_3)  //KEY1����PH3
#define KEY3        HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_5)  //KEY1����PH5

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define KEY3_PRES	4

void KEY_Init(void);
u8 KEY_Scan(u8 mode);
#endif
