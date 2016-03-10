#include "stm32l1xx_hal.h"
#include "usart.h"
#include "uartapp.h"
#include "devprotocol.h"
#include "stdio.h"

u8 uart1revbyte[REVBYTES];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//	RevdataWrFifo(uart1revbyte, REVBYTES);
	HAL_UART_Receive_IT(&huart1, uart1revbyte, REVBYTES);
//	HAL_UART_Transmit_IT(&huart1, uart1revbyte, REVBYTES);
}

int fputc (int c, FILE *fp)
{
	u8 temp = 0;
	temp = c;
	HAL_UART_Transmit(&huart1, &temp, 1, 1000);
	
	return c;
}

//void Uartx_IRQHandler_Idle(UART_HandleTypeDef *huart)
//{
//	uint8_t tmp = 0;
//	
//	tmp = huart->Instance->DR;
//	tmp = huart ->Instance->SR;
//	huart->State = HAL_UART_STATE_READY;
//	__HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
//	
//	if(&huart1 == huart)
//	{
//		HAL_UART_Receive_IT(&huart1, Uart1ReceiveBuffer, UART1RECEIVEPACKSIZE);
//		Uart1Para.Uart1ReceiveState = STATEDONE;
//	}
//	__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
//}

