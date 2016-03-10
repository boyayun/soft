/******************************************************************************
 * (c) Copyright 2010-2013, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * lcd_test.c
 ******************************************************************************/
#include "drivers.h"
#include "LCD.h"
#include "include.h"
#include "stdio.h"
	 
/* LEDs definition                                                            */
#define LED_G  PIN5        /* PTE5 */ 
#define LED_R  PIN1        /* PTF1 */ 
#define LED_Y  PIN1        /* PTD1 */ 
#define LED_O  PIN1        /* PTC1 */ 

#define DELAY()                 { arch_delay(100000); } 

//static const char digits[]=
//{
//  '0','1','2','3','4','5','6','7','8','9','A','b','c','d','E','F','8'
//};
//
//static uint32 tmp32;
//volatile static uint16 tmp16;
//static uint8  tmpNum;
//static uint8  led_state = 0;

volatile char c;
 static void uart_callback (UART_CALLBACK_SRC module, UART_CALLBACK_TYPE type, int8 status);
 
 void GPTimerEventHander(TMR_CH_CALLBACK_TYPE type, int32 result);
void GeneralPurposeTimerInit(void);
uint16 OneSecTimer=0;

void DevRevCmdHandle1();
#define TIMER_MOD                 1
/******************************************************************************/
void main (void)
{
  /* initialize SIM module                                                    */
  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);
  
//  /* clock mode 2:1:1, 48MHz                                                  */
//  SIM_SetClkMode (SYSCLK_MODE1); 
//  SIM_SetClkDiv  (SYSCLK_DIV1);
  
   /* clock mode 1:1:1, 24MHz  */
  SIM_SetClkMode (SYSCLK_MODE0); 
  SIM_SetClkDiv  (SYSCLK_DIV1);  
  FLL_Init  (FLL_MODULE_FEE_24MHZ_CONFIG);
//  ADC_Init (ADC_MODULE_16B_SWTRG_XREF_CONFIG, HWAVG_32, ADC_CH_SE_POLL_CONFIG(AD_OFF),
//            ADC_CH_DISABLE_CONFIG, ADC_CH_DISABLE_CONFIG, ADC_CH_DISABLE_CONFIG, 
//            0, NULL);
//  
//  LCD_Init (LCD_CHARGEPUMP_EN_SOURCE_ALT_BACKPLANES_8,                 \
//            LCD_FRONT_PLANE_PINS_TWR, LCD_BACK_PLANE_PINS_TWR,         \
//            LCD_ASSIGN_BACK_PLANES_TWR);
// 
//  PORT_Init (PORTE, PORT_MODULE_LED_MODE, LED_G);
//  PORT_Init (PORTF, PORT_MODULE_LED_MODE, LED_R);
//  PORT_Init (PORTD, PORT_MODULE_LED_MODE, LED_O);
//  PORT_Init (PORTC, PORT_MODULE_LED_MODE, LED_Y);
//  GPIO_Init (GPIOE, GPIO_OUT_LOGIC1_MODE, LED_G);
//  GPIO_Init (GPIOF, GPIO_OUT_LOGIC1_MODE, LED_R);
//  GPIO_Init (GPIOD, GPIO_OUT_LOGIC1_MODE, LED_O);
//  GPIO_Init (GPIOC, GPIO_OUT_LOGIC1_MODE, LED_Y);
//  GPIO_Set  (GPIOC, LED_Y);
//  GPIO_Set  (GPIOE, LED_G);
//  GPIO_Set  (GPIOF, LED_R);
//  GPIO_Set  (GPIOD, LED_O);
//  
//  /* test ON/OFF segments                                                     */
//  DELAY();
//  LCD_Write_Segments(LCD_ALL_SEGMENTS_ON_TWR);
//  DELAY(); DELAY(); DELAY(); DELAY(); DELAY(); DELAY(); DELAY(); DELAY();
//  LCD_Write_Segments(LCD_ALL_SEGMENTS_OFF_TWR);
//  DELAY(); DELAY(); DELAY(); DELAY(); DELAY(); DELAY(); DELAY(); DELAY();
//
//  _LCD_CHAR_V_S3_ON;
//  _LCD_CHAR_V_S6_ON;
//  _LCD_CHAR_V_S7_ON;
//  _LCD_CHAR_V_S9_ON;
//  _LCD_CHAR_V_S11_ON;
//  _LCD_CHAR_V_S12_ON;
//  _LCD_CHAR_V_S13_ON;
// 
//  _LCD_DOT_P3_ON;

//  while (1)
//  {
//    ADC_Start(CHA,AD10);
//    while (!ADC_Ready (CHA));
//    tmp16 = ADC_Read(CHA);
//    tmp32 = 50050 * tmp16;
//  
//    tmpNum = (uint8) (tmp32/1000000000);
//    tmp32 = tmp32 % 1000000000;
//    LCD_Delete_Char(3);
////	LCD_Write_Char ( 0x32 );
//    LCD_Write_Char ( digits[tmpNum] );
//
//    tmpNum = (uint8) (tmp32/100000000);
//    tmp32 = tmp32 % 100000000;
//    LCD_Delete_Char(2);
//    LCD_Write_Char ( digits[tmpNum] );
//
//    tmpNum = (uint8) (tmp32/10000000);
//    tmp32 = tmp32 % 10000000;
//    LCD_Delete_Char(1);
//    LCD_Write_Char ( digits[tmpNum] );
//  
//    tmpNum = (uint8) (tmp32/1000000);
//
//    LCD_Delete_Char(0);
//    LCD_Write_Char ( digits[tmpNum] );
//    
//    switch(led_state)
//    {
//      case 0:
//         GPIO_Clr  (GPIOE, LED_G);
//         GPIO_Set  (GPIOC, LED_Y);
//         led_state = 1;
//         break;
//         
//      case 1:
//         GPIO_Clr  (GPIOF, LED_R);
//         GPIO_Set  (GPIOE, LED_G);
//         led_state = 2;
//         break;
//    
//      case 2:
//         GPIO_Clr  (GPIOD, LED_O);
//         GPIO_Set  (GPIOF, LED_R);
//         led_state = 3;
//         break;
//
//      case 3:
//         GPIO_Clr  (GPIOC, LED_Y);
//         GPIO_Set  (GPIOD, LED_O);
//         led_state = 0;
//         break;
//    }
//    DELAY();
//    DELAY();
//    DELAY();
//    DELAY();
//  } 
  
  /* UART1 init 9600bd                                                        */
  PORT_Init   (PORTI, PORT_MODULE_ALT2_MODE, PIN0|PIN1);
  UART_Init   (UART1, UART_MODULE_INTRMODE_CONFIG(9600, 24e6));
  UART_InstallCallback (UART0_UART1, PRI_LVL1, uart_callback);
  
  /*Initializing the GPIO in Output mode to check the Timer functionality     */
  PORT_Init(PORTD,PORT_MODULE_LED_MODE,PIN1);
  GPIO_Init(GPIOD,GPIO_OUT_LOGIC0_MODE,PIN1);
  
  GeneralPurposeTimerInit();
  OneSecTimer=1000; /* This will be one second Timer. =40*25=1000ms             */
  
  EnableInterrupts();  
  
 // UART_PutStr(UART1,"\n\rCharacters will be echoed on the screen:\n");
  
  printf("ÄãºÃ,ÊÀ½ç\r\n");
  HardInit();
  ParaInit();
  while(1)
  {
  	MainHandle();
  }
}

int fputc (int c, FILE *fp)
{
	while (!UART_TxIdle(UART1));
	UART_PutChar (UART1, c);

	return c;
}

/* uart callback function definition                                          */
static void uart_callback (UART_CALLBACK_SRC module, UART_CALLBACK_TYPE type, int8 status)
{
  if (module == UART1_CALLBACK)
  {
    if (type == TX_CALLBACK) 
	{
		if(0 != modsendfifo.Datalen)
		{
			u8 x;
			FifoOutput(&x, &modsendfifo, 1);
			UART_PutChar (UART1, x); 
		}
		else
		{
			UART_TxIsrDisable (UART1);
		}
	}
	
    if (type == RX_CALLBACK) 
	{ 
		u8 tempchar;
		tempchar=UART_GetChar  (UART1); 
		RevDataWrFifo(&tempchar, 1);
//		UART_TxIsrEnable  (module); 
	}
  }
}

/*
  Quad Timer will be used as a General Purpose Timer.
  Presently it will be configured for 25 ms interrupt frequency.
  Using this Timer various Timer counter can be derived.
 */
void GeneralPurposeTimerInit()
{
  uint16 ModValue = 0;
  uint32 BusClk=24000000;
  /*
  Initializing the Configurable Timer
  It will use Quad Timer 1
  Enable the Quad Timer Clock
  Interrupt Frequency Desired= 25ms
  Interrupt Frequency in terms of Bus Clock= (Interrupt Time*Bus Freq)
  Configuring the Timer in Free Running Mode
  */
  ModValue = BusClk/1000 ;
  ModValue = ModValue / 128;
  ModValue=ModValue* TIMER_MOD;
  TMR_CH1_Init(TMR_CH_CNTR_CONFIG4(BUS_CLK_DIV128,SEC_CNTR0_INP),
      ModValue, 0x0000, 0x0000, ModValue, 0x0000, 1,
      &GPTimerEventHander);
}

void GPTimerEventHander(TMR_CH_CALLBACK_TYPE type, int32 result)
{
  if(type==COMPARE_CALLBACK)
  {
    /*Toggling GPIO at a frequency of 1sec    */
	 HAL_SYSTICK_Callback(); 
    if(OneSecTimer)
    {
      OneSecTimer--;
      if(OneSecTimer==0)
      {
        GPIO_Tgl(GPIOD,PIN1);
        OneSecTimer=1000;
      }
    }
  }
}
