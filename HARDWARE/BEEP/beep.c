#include "beep.h"



/**
 * @brief:		蜂鸣器初始化
 * @param:		none
 * @retval:		none
 */
void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOC_CLK_ENABLE();           //开启GPIOC时钟
	GPIO_Initure.Pin=GPIO_PIN_1; 			//PC1
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
	GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
	GPIO_Initure.Speed=GPIO_SPEED_MEDIUM ;  //中速
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
}

    
/**
 * @brief:		蜂鸣器鸣叫
 * @param:		none
 * @retval:		none
 */
void BEEP_ONCE(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);		//蜂鸣器对应引脚PC1拉低，
}
