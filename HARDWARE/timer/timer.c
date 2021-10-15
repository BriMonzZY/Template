#include "timer.h"


uint16_t tim_cnt_20 = 0;
uint16_t tim_cnt_15 = 0;
uint16_t tim_cnt_10 = 0;
uint16_t tim_cnt_5 = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim5.Instance){
		tim_cnt_10++;
		/* 10ms */
		if(tim_cnt_10 >= 10) {
			tim_cnt_10 = 0;
			printf("%d, %d, %d, %d\n", Read_Encoder(1), Read_Encoder(2), Read_Encoder(3), Read_Encoder(4));
		}
	}
}
