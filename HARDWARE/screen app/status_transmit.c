#include "status_transmit.h"



/* �л�״̬��ʱ����Ҫ:
   �л�״̬��״̬
   (�л�ipsPointerPosition)
   �����Ļ�����ƶ�Ӧ״̬����Ļ
*/


void Init_To_Manual(void)
{
	staSystem = MANUAL;
	Draw_MAUAL_Screen();
	
	ipsPointerPosition = 1;
	
}



void Init_To_Auto1(void)
{
	staSystem = AUTO1;
	Draw_AUTO1_Screen();
	
	ipsPointerPosition = 1;
}

void Init_To_Auto2(void)
{
	staSystem = AUTO2;
}

void Init_To_Follow(void)
{
	staSystem = FOLLOW;
}


void Auto1_To_Init(void)
{
	staSystem = INIT;
	Draw_INIT_Screen();
	
	ipsPointerPosition = 1;
}

void Auto2_To_Init(void)
{
	staSystem = INIT;
	Draw_INIT_Screen();
	
	ipsPointerPosition = 1;
}

void Follow_To_Init(void)
{
	staSystem = INIT;
	Draw_INIT_Screen();
	
	ipsPointerPosition = 1;
}

void Manual_To_Init(void)
{
	staSystem = INIT;
	Draw_INIT_Screen();
	
	ipsPointerPosition = 1;
}
