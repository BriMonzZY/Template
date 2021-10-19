#include "control.h"


/**
 * @brief:		������λ��ʽPID
 * @param:		PID *pp: PID�ṹ��
 *				double NextPoint:  ��ǰֵ
 * @retval:		����PID��ķ���ֵ
 * @author:		Brimon
 */
double PIDCalc(PID *pp, double NextPoint)   
{
	double dError,                              /* ��ǰ΢�� */
		   Error;                               /* ƫ�� */
	Error = pp->SetPoint - NextPoint;           /* ƫ��ֵ=�趨ֵ-����ֵ����ǰֵ��*/
	
	#if 0
		/* �����޷� */
		if(pp->SumError > 4000) pp->SumError = 4000;
		if(pp->SumError < -4000) pp->SumError = -4000;
	#endif
	
	pp->SumError += Error;                      /* ����=����+ƫ��  --ƫ����ۼ� */
	dError = pp->LastError - pp->PrevError;     /* ��ǰ΢�� = ������ - ֮ǰ��� */
	pp->PrevError = pp->LastError;              /* ���¡�֮ǰ�� */
	pp->LastError = Error;                      /* ���¡������ */
	return (pp->Kp * Error                      /* ������ = �������� * ƫ�� */
			+   pp->Ki *  pp->SumError              /* ������ = ���ֳ��� * ������ */
			+   pp->Kd * dError                     /* ΢���� = ΢�ֳ��� * ��ǰ΢�� */
			);
}

/**
 * @brief:		����������ʽPID
 * @param:		PID *pp: PID�ṹ��
 *				double NextPoint:  ��ǰֵ
 * @retval:		����PID�������ֵ
 * @author:		Brimon
 */
double PIDCalcIncrement(PID *pp, double NextPoint)   
{
	double Error;                               /* ƫ�� */
	Error = pp->SetPoint - NextPoint;           /* ƫ��ֵ=�趨ֵ-����ֵ����ǰֵ��*/
	
	pp->PrevError = pp->LastError;              /* ���¡�֮ǰ�� */
	pp->LastError = Error;                      /* ���¡������ */
	return (pp->Kp * (Error - pp->LastError)                      /* ������ = �������� * ƫ�� */
			+	pp->Ki * Error
			+   pp->Kd * (Error - 2*pp->LastError + pp->PrevError)                 /* ΢���� = ΢�ֳ��� * ��ǰ΢�� */
			);
}



