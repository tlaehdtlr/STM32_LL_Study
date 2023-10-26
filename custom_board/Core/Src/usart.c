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
#include <stdio.h>
#include <string.h>
#include "cli.h"

int _write(int file, char *ptr, int len)
{
  uart_transmit_it(USART3, (uint8_t*)ptr, len);
  return len;
}


#define UART_BUF_SIZE   256
#define UART_TX_Q_SIZE  32

uint8_t uart_tx_buf[UART_BUF_SIZE];
volatile uint16_t tx_w_idx = 0;
volatile uint16_t tx_r_idx = 0;

uint8_t uart_tx_que[UART_TX_Q_SIZE];
volatile uint8_t tx_que_w_idx = 0;
volatile uint8_t tx_que_r_idx = 0;

volatile bool tx_completed = true;
volatile uint16_t tx_size = 0;

uint8_t uart_rx_buf[UART_BUF_SIZE];
volatile uint16_t rx_idx = 0;
volatile uint16_t read_idx = 0;

/* USER CODE END 0 */

/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  /**USART3 GPIO Configuration
  PC10   ------> USART3_TX
  PC11   ------> USART3_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USART3 interrupt Init */
  NVIC_SetPriority(USART3_8_IRQn, 0);
  NVIC_EnableIRQ(USART3_8_IRQn);

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART3, &USART_InitStruct);
  LL_USART_DisableIT_CTS(USART3);
  LL_USART_ConfigAsyncMode(USART3);
  LL_USART_Enable(USART3);
  /* USER CODE BEGIN USART3_Init 2 */

  LL_USART_ClearFlag_ORE(USART3);
  LL_USART_EnableIT_RXNE(USART3);
  LL_USART_EnableIT_ERROR(USART3);

  /* USER CODE END USART3_Init 2 */

}

/* USER CODE BEGIN 1 */

/**
 * If Queue is not empty, override previous component
*/
void uart_transmit_it(USART_TypeDef *USARTx, uint8_t *p_value, uint16_t size)
{
  for (uint8_t idx = 0; idx < size; idx++)
  {
    uart_tx_buf[(tx_w_idx + idx) & (UART_BUF_SIZE-1) ] = *(p_value++);
  }
  tx_w_idx += size;
  uart_tx_que[tx_que_w_idx++ & (UART_TX_Q_SIZE-1)] = size;
}

void uart_txe_callback(USART_TypeDef *USARTx)
{
  if (tx_size == 1)
  {
    /* last byte */
    LL_USART_DisableIT_TXE(USARTx);
    LL_USART_EnableIT_TC(USARTx);
  }
  tx_size--;
  LL_USART_TransmitData8(USARTx, uart_tx_buf[tx_r_idx++ & (UART_BUF_SIZE-1)]);
}

void uart_tc_callback(USART_TypeDef *USARTx)
{
  tx_completed = true;
  LL_USART_DisableIT_TC(USART3);
}

void uart_idle(void)
{
  if (rx_idx != read_idx)
  {
    // printf("%c", uart_rx_buf[read_idx++]);
    LL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
    uart_transmit_it(USART3, &uart_rx_buf[read_idx++ & (UART_BUF_SIZE-1)], 1);
    // printf("rx idx %d, read idx %d \r\n", rx_idx, read_idx);
    // cli_read_character(uart_rx_buf[read_idx++ & (UART_BUF_SIZE-1)]);
  }


  if (tx_completed)
  {
    if (tx_que_w_idx != tx_que_r_idx)
    {
      tx_size = uart_tx_que[tx_que_r_idx++ & (UART_TX_Q_SIZE - 1)];
      tx_completed = false;
      LL_USART_EnableIT_TXE(USART3);
    }
  }
}

void uart_receive_callback(USART_TypeDef *USARTx)
{
  uart_rx_buf[rx_idx++ & (UART_BUF_SIZE-1)] = LL_USART_ReceiveData8(USARTx);
}

void UART_Error_Callback(void)
{
  __IO uint32_t isr_reg;

  /* Disable USARTx_IRQn */
  NVIC_DisableIRQ(USART3_8_IRQn);
  
  /* Error handling example :
    - Read USART ISR register to identify flag that leads to IT raising
    - Perform corresponding error handling treatment according to flag
  */
  isr_reg = LL_USART_ReadReg(USART3, ISR);
  if (isr_reg & LL_USART_ISR_NE)
  {
    /* Transfer error in reception/transmission process */
  }
  else
  {
    /* Turn LED2 on: Transfer error in reception/transmission process */
  }
  LL_GPIO_SetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
}



/* USER CODE END 1 */
