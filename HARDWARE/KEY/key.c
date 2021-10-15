#include "key.h"

/**
 * @brief:		������ʼ��
 * @param:		none
 * @retval:		none
 */
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
    __HAL_RCC_GPIOH_CLK_ENABLE();           //����GPIOHʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_2;            //PA2
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5; //PH2,3
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	HAL_GPIO_Init(GPIOH,&GPIO_Initure);
}

/**
 * @brief:		��������
 * @param:		����ģʽ
 *				1, ֧������
 * @retval:		���°����ļ�ֵ
 *				0, û���κΰ�������
 */
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;     //�����ɿ���־
    if(mode==1)key_up=1;    //֧������
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
    return 0;   //�ް�������
}