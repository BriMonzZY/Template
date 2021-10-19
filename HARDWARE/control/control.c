#include "control.h"


/**
 * @brief:		完整的位置式PID
 * @param:		PID *pp: PID结构体
 *				double NextPoint:  当前值
 * @retval:		经过PID后的返回值
 * @author:		Brimon
 */
double PIDCalc(PID *pp, double NextPoint)   
{
	double dError,                              /* 当前微分 */
		   Error;                               /* 偏差 */
	Error = pp->SetPoint - NextPoint;           /* 偏差值=设定值-输入值（当前值）*/
	
	#if 0
		/* 积分限幅 */
		if(pp->SumError > 4000) pp->SumError = 4000;
		if(pp->SumError < -4000) pp->SumError = -4000;
	#endif
	
	pp->SumError += Error;                      /* 积分=积分+偏差  --偏差的累加 */
	dError = pp->LastError - pp->PrevError;     /* 当前微分 = 最后误差 - 之前误差 */
	pp->PrevError = pp->LastError;              /* 更新“之前误差” */
	pp->LastError = Error;                      /* 更新“最后误差” */
	return (pp->Kp * Error                      /* 比例项 = 比例常数 * 偏差 */
			+   pp->Ki *  pp->SumError              /* 积分项 = 积分常数 * 误差积分 */
			+   pp->Kd * dError                     /* 微分项 = 微分常数 * 当前微分 */
			);
}

/**
 * @brief:		完整的增量式PID
 * @param:		PID *pp: PID结构体
 *				double NextPoint:  当前值
 * @retval:		经过PID后的增量值
 * @author:		Brimon
 */
double PIDCalcIncrement(PID *pp, double NextPoint)   
{
	double Error;                               /* 偏差 */
	Error = pp->SetPoint - NextPoint;           /* 偏差值=设定值-输入值（当前值）*/
	
	pp->PrevError = pp->LastError;              /* 更新“之前误差” */
	pp->LastError = Error;                      /* 更新“最后误差” */
	return (pp->Kp * (Error - pp->LastError)                      /* 比例项 = 比例常数 * 偏差 */
			+	pp->Ki * Error
			+   pp->Kd * (Error - 2*pp->LastError + pp->PrevError)                 /* 微分项 = 微分常数 * 当前微分 */
			);
}



