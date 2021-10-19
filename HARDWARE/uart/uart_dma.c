#include "uart_dma.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "numandstr.h"

uint8_t RxBuffer[UART_RX_BUF_SIZE] = {0};
uint8_t TxBuffer[UART_RX_BUF_SIZE] = {0};
uint8_t sendCompleteSign = 1;
uint8_t TxLen = 0;


int stack_push(STACK *stack, unsigned char data);
int stack_read_top(STACK *stack);
int stack_pop(STACK *stack);

#if 0
/**
 * @brief:		��������ľ�������   ֱ�ӽ�����ֵ��ֵ��distance  ����ʽ����
 * @param:		none
 * @retval:		none
 * @author:		Brimon
 */
void Distance_Process(void)
{
	unsigned int zero[100] = {0};
	uint16_t distance_tmp = 0, i;
	unsigned int distance_tmp_cnt = 0;
	
	HAL_UART_Receive(&huart6, temp_uart6, 64, 1000);  /* �������� */
	
	/*
	for(i = 0; i < 64; i++) {
		printf("%d ", temp_uart6[i]);
	}
	printf("\n");
	*/
	
	distance_tmp = 0;
	for(i = 0; i < 64; i++) {
		/* d: 1405 mm
			 100 58 32 49 52 48 53 32 109 109
		*/
		if(temp_uart6[i] == 100 && temp_uart6[i+1] == 58 && temp_uart6[i+2] == 32) {
			stack_push(&distance_stack, temp_uart6[i]);
			stack_push(&distance_stack, temp_uart6[i+1]);
			stack_push(&distance_stack, temp_uart6[i+2]);
		}
		/* ���վ�����Ϣ */
		if(distance_stack.stack[0] == 100 && distance_stack.stack[1] == 58 && temp_uart6[i] != 109 && temp_uart6[i+1] != 109) {
			stack_push(&distance_stack, temp_uart6[i]);
		}
		/* �������� */
		if(distance_stack.stack[0] == 100 && distance_stack.stack[1] == 58 && temp_uart6[i] == 109 && temp_uart6[i+1] == 109) {
			stack_push(&distance_stack, temp_uart6[i]);
			stack_push(&distance_stack, temp_uart6[i+1]);
			
			stack_pop(&distance_stack);
			stack_pop(&distance_stack);
			stack_pop(&distance_stack);
		
			while(distance_stack.stack[distance_stack.stack_top] != 32) {  /* ���ݼ��� */
				//printf("%d ", distance_stack.stack[distance_stack.stack_top]-48);
				distance_tmp = distance_tmp + (distance_stack.stack[distance_stack.stack_top]-48)*pow(10, distance_tmp_cnt++);
				stack_pop(&distance_stack);
			}
			if(distance_stack.stack[distance_stack.stack_top] == 32) {  /* ���������ȴ�������һ�� */
				memcpy(distance_stack.stack, zero, 100);
				distance_stack.stack_top = 0;
				distance_tmp_cnt = 0;
				distance = distance_tmp;
				//printf("distance: %d\n\n", distance);
				distance_tmp = 0;
			}
			if(distance_stack.stack[0] == 100 && distance_stack.stack[1] == 58 && i == 63) {  /* û�н�β���ݲ����� */
				memcpy(distance_stack.stack, zero, 100);
				distance_stack.stack_top = 0;
			}
		}
	}
}

#endif



/* ���ݴ����� */
/* �Կո���Ϊ�ָ�����RxBuffer�Ǵ��ڽ��ջ������飬�����Ķ������� */
int32_t a,b,c;
double  d,e,f;
void DataProcess(void)
{
	//������������ݴ���ĺ���
	a = str2int(RxBuffer, ' ', 1);
	b = str2int(RxBuffer, ' ', 2);
	c = str2int(RxBuffer, ' ', 3);
	str2double(RxBuffer, ' ', 4, &d);
	str2double(RxBuffer, ' ', 5, &e);
	str2double(RxBuffer, ' ', 6, &f);
}


void HAL_UART_IdleCallback(UART_HandleTypeDef *huart)
{
	__HAL_UART_CLEAR_IDLEFLAG(huart);
	{
		HAL_UART_DMAStop(huart);

        ProcessData();

        StartUartRxDMA();
	}
}

void ProcessData()
{
    uint32_t len = 0;

    //�õ��Ѿ������˶��ٸ��ֽ� = �ܹ�Ҫ���յ��ֽ��� - ?NDTR F1ΪCNDTR F4ΪNDTR
    #ifdef __STM32F1xx_HAL_H
    	len = UART_RX_BUF_SIZE - USB_Huart.hdmarx->Instance->CNDTR;
    	#define ProcessDataOK
    #endif

    #ifdef  __STM32F4xx_HAL_H
        len = UART_RX_BUF_SIZE - USB_Huart.hdmarx->Instance->NDTR;
    	#define ProcessDataOK
    #endif

    #ifndef ProcessDataOK
    	��������оƬ�İ汾
    #endif

    if(len > 0)
    {
        if(sendCompleteSign == 1)
        {
#if UART_RXTX_Switch
            memset((void *)TxBuffer, 0, sizeof(TxBuffer));
            memcpy(TxBuffer, RxBuffer, len);
            TxLen = len;
            StartUartTxDMA();	//���ڻ���
#endif
            {
            	//��������������ݴ���ĺ���
            	DataProcess();
            }
        }
    }
}

void USB_DMA_printf(const char *format,...)
{
	uint32_t length;
	va_list args;

	va_start(args, format);
	length = vsnprintf((char*)TxBuffer, sizeof(TxBuffer)+1, (char*)format, args);
	va_end(args);

	HAL_UART_Transmit_DMA(&USB_Huart,TxBuffer,length);
}

void USB_printf(const char *format,...)
{
	uint32_t length;
	va_list args;

	va_start(args, format);
	length = vsnprintf((char*)TxBuffer, sizeof(TxBuffer)+1, (char*)format, args);
	va_end(args);

	HAL_UART_Transmit(&USB_Huart,TxBuffer,length,0xFFFF);
}




/**
  * @brief  Tx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
//  UNUSED(huart);
    if(huart == &USB_Huart)
    {
        sendCompleteSign = 1;
    }
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
	//  UNUSED(huart);
	if(huart == &USB_Huart)
	{
			ProcessData();

			StartUartRxDMA();
	}
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
  */
}

uint8_t UartTxData(UART_HandleTypeDef *huart, uint8_t *buf, const uint32_t len)
{
	HAL_StatusTypeDef status;
	uint8_t ret = 1;

	if(sendCompleteSign == 0 || len == 0)
	{
		return 0;
	}

    sendCompleteSign = 0;

	status = HAL_UART_Transmit_DMA(huart, (uint8_t*)buf, len);

	if(HAL_OK != status)
	{
		ret = 0;
	}

	return ret;
}

//����DMA����
uint8_t StartUartTxDMA()
{
    return UartTxData(&USB_Huart, TxBuffer, TxLen);
}

uint8_t UartRxData(UART_HandleTypeDef *huart, uint8_t *buf, const uint32_t len)
{
	HAL_StatusTypeDef status;
	uint8_t ret = 1;

	status = HAL_UART_Receive_DMA(huart, (uint8_t*)buf, len);
	if(HAL_OK != status)
	{
		ret = 0;
	}
	else
	{
		/* �������н����ж� */
	    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
	}

	return ret;
}

//����DMA����
uint8_t StartUartRxDMA()
{
    return UartRxData(&USB_Huart, RxBuffer, UART_RX_BUF_SIZE);
}
