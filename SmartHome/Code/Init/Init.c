//#include "stm32l1xx_hal.h"
//#include "usart.h"
//#include "gpio.h"
#include "uartapp.h"
#include "stdio.h"

#ifndef DEVEICE
	#include "devproanaly.h"
#else
	#include "deveice.h"
#endif

/******************************************
Fun: Ӳ����ʼ������
Input: NULL
Output:NULL
Return:NULL
******************************************/
void HardInit()
{
	DevHardInit();
}

/******************************************
Fun: ������ʼ������
Input: NULL
Output:NULL
Return:NULL
******************************************/
void ParaInit()
{
	DevParaInit();
}
