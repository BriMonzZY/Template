#include "motor.h"


int Read_Encoder(uint8_t TIMx)
{
	int Encoder_TIM;
	switch(TIMx) {
		case 1: Encoder_TIM= (short)TIM1 -> CNT;  TIM1 -> CNT=0;break;
		case 2: Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
		case 3: Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;
		case 4: Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;
		case 5: Encoder_TIM= (short)TIM5 -> CNT;  TIM5 -> CNT=0;break;
		case 6: Encoder_TIM= (short)TIM6 -> CNT;  TIM6-> CNT=0;break;
		case 7: Encoder_TIM= (short)TIM7 -> CNT;  TIM7 -> CNT=0;break;
		case 8: Encoder_TIM= (short)TIM8 -> CNT;  TIM8 -> CNT=0;break;
		#if 1
		case 9: Encoder_TIM= (short)TIM9 -> CNT;  TIM9 -> CNT=0;break;
		case 10: Encoder_TIM= (short)TIM10 -> CNT;  TIM10 -> CNT=0;break;
		case 11: Encoder_TIM= (short)TIM11 -> CNT;  TIM11 -> CNT=0;break;
		case 12: Encoder_TIM= (short)TIM12 -> CNT;  TIM12 -> CNT=0;break;
		case 13: Encoder_TIM= (short)TIM13 -> CNT;  TIM13 -> CNT=0;break;
		case 14: Encoder_TIM= (short)TIM14 -> CNT;  TIM14 -> CNT=0;break;
		#endif
	}
	return Encoder_TIM;
}

