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
			
			#if defined(USE_ENCODER)
				printf("%d, %d, %d, %d\n", Read_Encoder(1), Read_Encoder(2), Read_Encoder(3), Read_Encoder(4));
			#endif
			
			#if defined(USE_ICM20602)
				get_icm20602_accdata_simspi();
				get_icm20602_gyro_simspi();
				IMU_quaterToEulerianAngles();  // ªÒ»°≈∑¿≠Ω«
				printf("%.2f,%.2f,%.2f\n", eulerAngle.pitch, eulerAngle.roll, eulerAngle.yaw);
			#endif
			
		}
	}
}
