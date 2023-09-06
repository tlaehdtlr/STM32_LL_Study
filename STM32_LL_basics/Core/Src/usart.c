/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <stdbool.h>
#include <stdio.h>
#define DEBUG_USART     USART1
#define UART_RX_BUF_SIZE    256

volatile uint8_t uart1_idle = 0;

static uint8_t              uart1_rx_buf[UART_RX_BUF_SIZE] = {0,};
static volatile uint32_t    uart1_rx_idx = 0;
static volatile uint32_t    uart1_read_idx = 0;
static uint8_t              cmd[UART_RX_BUF_SIZE] = {0,};
static volatile uint8_t     cmd_idx = 0;


volatile uint8_t uart2_idle = 0;

static uint8_t              uart2_rx_buf[UART_RX_BUF_SIZE] = {0,};
static volatile uint32_t    uart2_rx_idx = 0;
static volatile uint32_t    uart2_read_idx = 0;




/* USER CODE END 0 */

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, 0);
  NVIC_EnableIRQ(USART1_IRQn);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_DisableIT_CTS(USART1);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART2 GPIO Configuration
  PA2   ------> USART2_TX
  PA3   ------> USART2_RX
  */
  GPIO_InitStruct.Pin = USART_TX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(USART_TX_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = USART_RX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(USART_RX_GPIO_Port, &GPIO_InitStruct);

  /* USART2 interrupt Init */
  NVIC_SetPriority(USART2_IRQn, 0);
  NVIC_EnableIRQ(USART2_IRQn);

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_DisableIT_CTS(USART2);
  LL_USART_ConfigAsyncMode(USART2);
  LL_USART_Enable(USART2);
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/* USER CODE BEGIN 1 */
static bool process_cmd(uint8_t cmd_ch)
{
  /* check if valid character */
  if ((cmd_ch >= ' ') && (cmd_ch <= '~'))
  {
      cmd[cmd_idx++] = cmd_ch;
      printf("%c", cmd_ch);
      // printf("a");
      // LL_USART_TransmitData8(DEBUG_USART, cmd_ch);
  }

  /* delete if back space is pressed */
  if (cmd_ch == '\b')
  {
    if (cmd_idx)
    {
      cmd[--cmd_idx] = 0;
      printf("\b\x1B[K");
    }
  }
  /* do parsing command if input is enter key */
  if ((cmd_ch == '\r') || (cmd_ch == '\n'))
  {
    cmd[cmd_idx++] = cmd_ch;
    printf("\r\n");
    return true;
  }

  return false;
}


void uart_idle(void)
{
	if (uart1_idle)
	{
		LL_USART_DisableIT_RXNE(USART1);

    while (uart1_rx_idx != uart1_read_idx)
    {
      if (process_cmd(uart1_rx_buf[uart1_read_idx++ & (UART_RX_BUF_SIZE-1)]))
      {
        printf(" cmd : %s \r\n", cmd);
        while (cmd_idx)
        {
          cmd[--cmd_idx] = 0;
        }
      }
    }      

    LL_USART_EnableIT_RXNE(USART1);

		uart1_idle = 0;
	}

	if (uart2_idle)
	{
		LL_USART_DisableIT_RXNE(USART2);

		while (uart2_rx_idx != uart2_read_idx)
    {
      while(!LL_USART_IsActiveFlag_TXE(USART2));
      LL_USART_TransmitData8(USART2, uart2_rx_buf[uart2_read_idx++ & (UART_RX_BUF_SIZE-1)]);
    }

    LL_USART_EnableIT_RXNE(USART2);

		uart2_idle = 0;
	}
}

void uart_init(void)
{
  LL_USART_EnableIT_RXNE(USART1);
  LL_USART_EnableIT_IDLE(USART1);
  LL_USART_EnableIT_ERROR(USART1);
  
  LL_USART_EnableIT_RXNE(USART2);
  LL_USART_EnableIT_IDLE(USART2);
  LL_USART_EnableIT_ERROR(USART2);

  setbuf(stdout, NULL);
}


void uart_error_callback(USART_TypeDef *USARTx)
{
  LL_USART_DisableIT_RXNE(USARTx);
  LL_USART_DisableIT_IDLE(USARTx);
  LL_USART_DisableIT_ERROR(USARTx);

  // LL_USART_ClearFlag_RXNE(USARTx);
  LL_USART_ClearFlag_IDLE(USARTx);
  LL_USART_ClearFlag_FE(USARTx);
  LL_USART_ClearFlag_ORE(USARTx);
  LL_USART_ClearFlag_NE(USARTx);

  LL_USART_Disable(USARTx);

  //add your error handling here
}

void uart_irq_rx_callback(USART_TypeDef *USARTx)
{
  if (USARTx == USART1)
  {
    //read data register not exmpty
    if(LL_USART_IsActiveFlag_RXNE(USART1))
    {
      uart1_rx_buf[uart1_rx_idx++ & (UART_RX_BUF_SIZE-1)] = LL_USART_ReceiveData8(USART1);
      // LL_USART_ClearFlag_RXNE(USART2);
    }

    //idle
    if(LL_USART_IsActiveFlag_IDLE(USART1))
    {
      uart1_idle = 1;
      LL_USART_ClearFlag_IDLE(USART1);
    }
  }
  else if (USARTx == USART2)
  {
    //read data register not exmpty
    if(LL_USART_IsActiveFlag_RXNE(USART2))
    {
      uart2_rx_buf[uart2_rx_idx++ & (UART_RX_BUF_SIZE-1)] = LL_USART_ReceiveData8(USART2);
      // LL_USART_ClearFlag_RXNE(USART2);
    }

    //idle
    if(LL_USART_IsActiveFlag_IDLE(USART2))
    {
      uart2_idle = 1;
      LL_USART_ClearFlag_IDLE(USART2);
    }
  }


  // frame error
  if(LL_USART_IsActiveFlag_FE(USARTx))
  {
    uart_error_callback(USARTx);
  }

  // overrun error
  if(LL_USART_IsActiveFlag_ORE(USARTx))
  {
    uart_error_callback(USARTx);
  }

  // noise error
  if(LL_USART_IsActiveFlag_NE(USARTx))
  {
    uart_error_callback(USARTx);
  }
}



#if 1
int _write(int file, char* p, int len)
{
  for (int i=0; i<len; i++)
  {
    while(!LL_USART_IsActiveFlag_TXE(DEBUG_USART));
    LL_USART_ClearFlag_TC(DEBUG_USART);
    LL_USART_TransmitData8(DEBUG_USART, *(p+i));
  }
  while (LL_USART_IsActiveFlag_TC(DEBUG_USART));

  return len;
}
#else

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */

  /* Wait for TXE flag to be raised */
  while (!LL_USART_IsActiveFlag_TXE(DEBUG_USART))
  {
  }

  /* Write character in Transmit Data register.
     TXE flag is cleared by writing data in TDR register */
  LL_USART_TransmitData8(DEBUG_USART, ch);

  /* Wait for TC flag to be raised for last char */
  while (!LL_USART_IsActiveFlag_TC(DEBUG_USART))
  {
  }
  
  return ch;
}

#endif
/* USER CODE END 1 */
