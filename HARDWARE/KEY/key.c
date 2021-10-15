#include "key.h"

/**
 * @brief:		按键初始化
 * @param:		none
 * @retval:		none
 */
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
    __HAL_RCC_GPIOH_CLK_ENABLE();           //开启GPIOH时钟
    
    GPIO_Initure.Pin=GPIO_PIN_2;            //PA2
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5; //PH2,3
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	HAL_GPIO_Init(GPIOH,&GPIO_Initure);
}

/**
 * @brief:		按键处理
 * @param:		按键模式
 *				1, 支持连按
 * @retval:		按下按键的键值
 *				0, 没有任何按键按下
 */
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;     //按键松开标志
    if(mode==1)key_up=1;    //支持连按
    if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0))
    {
        delay_ms(10);
        key_up=0;
        if(KEY0==0)       return KEY0_PRES;
        else if(KEY1==0)  return KEY1_PRES;
		else if(KEY2==0)  return KEY2_PRES;
		else if(KEY3==0)  return KEY3_PRES;
    }
	else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1) key_up=1;
    return 0;   //无按键按下
}
