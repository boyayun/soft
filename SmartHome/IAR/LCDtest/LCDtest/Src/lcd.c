/**
  ******************************************************************************
  * File Name          : LCD.c
  * Description        : This file provides code for the configuration
  *                      of the LCD instances.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lcd.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

LCD_HandleTypeDef hlcd;

/* LCD init function */
void MX_LCD_Init(void)
{

  hlcd.Instance = LCD;
  hlcd.Init.Prescaler = LCD_PRESCALER_1;
  hlcd.Init.Divider = LCD_DIVIDER_31;
  hlcd.Init.Duty = LCD_DUTY_1_4;
  hlcd.Init.Bias = LCD_BIAS_1_3;
  hlcd.Init.VoltageSource = LCD_VOLTAGESOURCE_INTERNAL;
  hlcd.Init.Contrast = LCD_CONTRASTLEVEL_4;
  hlcd.Init.DeadTime = LCD_DEADTIME_0;
  hlcd.Init.PulseOnDuration = LCD_PULSEONDURATION_4;
  hlcd.Init.MuxSegment = LCD_MUXSEGMENT_ENABLE;
  hlcd.Init.BlinkMode = LCD_BLINKMODE_OFF;
  hlcd.Init.BlinkFrequency = LCD_BLINKFREQUENCY_DIV32;
  HAL_LCD_Init(&hlcd);

}

void HAL_LCD_MspInit(LCD_HandleTypeDef* hlcd)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hlcd->Instance==LCD)
  {
  /* USER CODE BEGIN LCD_MspInit 0 */

  /* USER CODE END LCD_MspInit 0 */
    /* Peripheral clock enable */
    __LCD_CLK_ENABLE();
  
    /**LCD GPIO Configuration    
    PC0     ------> LCD_SEG18
    PC1     ------> LCD_SEG19
    PC2     ------> LCD_SEG20
    PC3     ------> LCD_SEG21
    PA1     ------> LCD_SEG0
    PA2     ------> LCD_SEG1
    PA3     ------> LCD_SEG2
    PB10     ------> LCD_SEG10
    PB11     ------> LCD_SEG11
    PB12     ------> LCD_SEG12
    PB13     ------> LCD_SEG13
    PB14     ------> LCD_SEG14
    PB15     ------> LCD_SEG15
    PC6     ------> LCD_SEG24
    PC7     ------> LCD_SEG25
    PC8     ------> LCD_SEG26
    PC9     ------> LCD_SEG27
    PA8     ------> LCD_COM0
    PA9     ------> LCD_COM1
    PA10     ------> LCD_COM2
    PA15     ------> LCD_SEG17
    PC10     ------> LCD_SEG40
    PC11     ------> LCD_SEG41
    PB3     ------> LCD_SEG7
    PB4     ------> LCD_SEG8
    PB5     ------> LCD_SEG9
    PB8     ------> LCD_SEG16
    PB9     ------> LCD_COM3 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 
                          |GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_VERY_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF11_LCD;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_8 
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_VERY_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF11_LCD;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_4 
                          |GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_VERY_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF11_LCD;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(LCD_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(LCD_IRQn);
  /* USER CODE BEGIN LCD_MspInit 1 */

  /* USER CODE END LCD_MspInit 1 */
  }
}

void HAL_LCD_MspDeInit(LCD_HandleTypeDef* hlcd)
{

  if(hlcd->Instance==LCD)
  {
  /* USER CODE BEGIN LCD_MspDeInit 0 */

  /* USER CODE END LCD_MspDeInit 0 */
    /* Peripheral clock disable */
    __LCD_CLK_DISABLE();
  
    /**LCD GPIO Configuration    
    PC0     ------> LCD_SEG18
    PC1     ------> LCD_SEG19
    PC2     ------> LCD_SEG20
    PC3     ------> LCD_SEG21
    PA1     ------> LCD_SEG0
    PA2     ------> LCD_SEG1
    PA3     ------> LCD_SEG2
    PB10     ------> LCD_SEG10
    PB11     ------> LCD_SEG11
    PB12     ------> LCD_SEG12
    PB13     ------> LCD_SEG13
    PB14     ------> LCD_SEG14
    PB15     ------> LCD_SEG15
    PC6     ------> LCD_SEG24
    PC7     ------> LCD_SEG25
    PC8     ------> LCD_SEG26
    PC9     ------> LCD_SEG27
    PA8     ------> LCD_COM0
    PA9     ------> LCD_COM1
    PA10     ------> LCD_COM2
    PA15     ------> LCD_SEG17
    PC10     ------> LCD_SEG40
    PC11     ------> LCD_SEG41
    PB3     ------> LCD_SEG7
    PB4     ------> LCD_SEG8
    PB5     ------> LCD_SEG9
    PB8     ------> LCD_SEG16
    PB9     ------> LCD_COM3 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 
                          |GPIO_PIN_10|GPIO_PIN_11);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_8 
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_4 
                          |GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(LCD_IRQn);

  }
  /* USER CODE BEGIN LCD_MspDeInit 1 */

  /* USER CODE END LCD_MspDeInit 1 */
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
