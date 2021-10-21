/**
 * @project	
 * @author ���»���
 * @members	����� ����Ң ���
 * @brief ����
 * @version 0.1
 * @date 2021-8-4
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#define _MAIN_C
#include "main.h"

void globalInit(void);
/* ȫ�ֱ������� */

uint8_t temp_usart3;					/* uart3���ջ����� */
enum estaSystem staSystem = INIT;		/* ���ó�ʼ״̬staSystemΪINIT */
u16 screen_cnt = 0;						/* ���������� */
u8 keyValue = 0;						/* �������µļ�ֵ */
u16 ipsPointerPosition = 1;				/* ips��Ļ��ָ���λ��   ��1��ʼ */


/* ****************************************������*********************************** */

/**
 * @brief:		������
 * @param:		none
 * @retval:		none
 */
int main(void)
{
	/* ������������ */
	
	/* ��ʼ������ */
	globalInit();											/* ȫ�ֳ�ʼ�� */
	
/********************************************while(1)**********************************************/
	while(1) {
		/* ��ⰴ�� */
		keyValue = KEY_Scan(0);
		
		/* ������������ */
		#if defined(USE_LCD)
			tp_dev.scan(0);									/* ɨ�败���� */
			if(tp_dev.sta & TP_PRES_DOWN){					/* ������������ */
				if(tp_dev.sta & TP_PRES_DOWN){				/* ������������ */
					Button_App();							/* ���������¶��� �����л��ӡ���� */
					screen_cnt++;
					delay_ms(50);
				}
			}
			if(!(tp_dev.sta&TP_PRES_DOWN)) {   	/* ���������ɿ� */ 
				if(!(tp_dev.sta&TP_PRES_DOWN)){		/* ���������ɿ� */
					screen_cnt = 0;
				}
			}
		#endif
		
		/* ״̬���߼��ж� */
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
 * @brief:		ȫ�ֳ�ʼ��
 * @param:		none
 * @retval:		none
 */
void globalInit(void)
{
	HAL_Init();											/* ��ʼ��HAL�� */
	Stm32_Clock_Init(360,25,2,8);   					/* ����ʱ��,180MHz */
	delay_init(180);                					/* ��ʼ����ʱ���� 180MHz */
	MX_GPIO_Init();										/* ��ʼ��GPIO */
	MX_DMA_Init();										/* ��ʼ��DMA */
	MX_USART1_UART_Init();								/* ��ʼ��uart1 */
	MX_USART3_UART_Init();								/* ��ʼ��uart3 */
	MX_TIM5_Init();										/* ��ʼ����ʱ��TIM5 */
	StartUartRxDMA();  									/* ʹ��uartDMA(uart3) */
	HAL_UART_Receive_IT(&huart3, &temp_usart3, 1);  	/* ��uart�ж� */
	LED_Init();                     					/* ��ʼ��LED */
	KEY_Init();                     					/* ��ʼ������ */
	HAL_TIM_Base_Start_IT(&htim5);   					/* ��ʱ��5ʹ�� */
	BEEP_Init();										/* ��ʼ�������� */
	
	
	/* ��ʼ��LCD */
	#if defined(USE_LCD) && defined(NOT_USE_STEER) && defined(NOT_USE_STEER)
		SDRAM_Init();                   					/* ��ʼ��SDRAM*/
		LCD_Init();                     					/* ��ʼ��LCD */
		tp_dev.init();				    					/* ��������ʼ�� */ 
		POINT_COLOR=BLUE;									/* �ʴ���ɫ */
		Draw_INIT_Screen();									/* ��ʼ����ĻΪInit���� */
	#endif
	
	
	/* ��ʼ��IPS */
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
	
	
	/* ��ʼ����� */
	#if defined(USE_STEER)
		/* �������PWM��ʼ�� */
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
	
	
	/* ��ʼ����� */
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
	
	
	/* ��ʼ�������� */
	#if defined(USE_ENCODER)
		MX_TIM1_Init();
		MX_TIM2_Init();
		MX_TIM3_Init();
		MX_TIM4_Init();
		HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);  /* ����������ģʽ */
		HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
		HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
		HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
	#endif
	
	
	/* ��ʼ�������� */
	#if defined(USE_LASER_RANGING)
		
	#endif
	
	
	/* ��ʼ�������� */
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
