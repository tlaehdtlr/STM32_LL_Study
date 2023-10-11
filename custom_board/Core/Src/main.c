/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BLINKING_FAST   1200000
#define BLINKING_SLOW   4800000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
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
  while (!LL_USART_IsActiveFlag_TXE(USART3))
  {
  }

  /* Write character in Transmit Data register.
     TXE flag is cleared by writing data in TDR register */
  LL_USART_TransmitData8(USART3, ch);

  /* Wait for TC flag to be raised for last char */
  while (!LL_USART_IsActiveFlag_TC(USART3))
  {
  }
  
  return ch;
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile bool flag = false;
volatile uint32_t cnt = 0;
volatile uint8_t uart_receive_data = 0;
volatile uint32_t blinking_period = BLINKING_SLOW;
volatile uint32_t blinking_cnt = 100;

uint8_t greeting[] = "\r\nHello \r\n";
uint8_t new_line[] = "\r\n";
uint8_t *p_tx;
volatile uint16_t tx_idx = 0;
volatile uint16_t tx_size = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
#if 0
static void debug_uart_transmit(USART_TypeDef *USARTx, uint8_t value);
#endif
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, 3);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  LL_USART_EnableIT_TXE(USART3);
  LL_GPIO_ResetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
  printf("\r\nHello, this is a custom board LL example \r\n");
  while (1)
  {
    if (blinking_cnt++ - 100 > blinking_period)
    {
      blinking_cnt = 100;
      LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
    }
    if (cnt++ > 2400000)
    {
      cnt = 0;
      flag = !flag;
      if (flag)
      {
        LL_GPIO_SetOutputPin(LED_3_GPIO_Port, LED_3_Pin);
      }
      else
      {
        LL_GPIO_ResetOutputPin(LED_3_GPIO_Port, LED_3_Pin);
      }
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLL_MUL_2, LL_RCC_PREDIV_DIV_1);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(48000000);
  LL_SetSystemCoreClock(48000000);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1);
}

/* USER CODE BEGIN 4 */
#if 1
void uart_transmit_it(USART_TypeDef *USARTx, uint8_t *p_value, uint16_t size)
{
  tx_size = size;
  p_tx = p_value;
  LL_USART_TransmitData8(USARTx, p_tx[tx_idx++]);
  LL_USART_EnableIT_TXE(USARTx);
}

void uart_transmit_callback(USART_TypeDef *USARTx)
{
  if (tx_idx == tx_size)
  {
    tx_idx = 0;
    tx_size = 0;
    LL_USART_DisableIT_TXE(USARTx);
  }
  else
  {
    LL_USART_TransmitData8(USARTx, p_tx[tx_idx++]);
  }
}

#else
static void debug_uart_transmit(USART_TypeDef *USARTx, uint8_t value)
{
  while (!LL_USART_IsActiveFlag_TXE(USARTx))
  {
  }

  /* Write character in Transmit Data register.
     TXE flag is cleared by writing data in TDR register */
  LL_USART_TransmitData8(USARTx, value);

  /* Wait for TC flag to be raised for last char */
  while (!LL_USART_IsActiveFlag_TC(USARTx))
  {
  }
}
#endif

void debug_uart_receive(USART_TypeDef *USARTx)
{
  uart_receive_data = LL_USART_ReceiveData8(USARTx);
#if 1
  uart_transmit_it(USARTx, &uart_receive_data, 1);
#else
  debug_uart_transmit(USARTx, uart_receive_data);
#endif
  switch (uart_receive_data)
  {
    case 0: return;
    case '1':
    {
      LL_GPIO_SetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
    }
      break;
    case '2':
    {
      LL_GPIO_ResetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
    }
      break;
    case '3':
    {
      blinking_period = BLINKING_SLOW;
    }
      break;
    case '4':
    {
      blinking_period = BLINKING_FAST;
    }
      break;
    case '5':
    {
      blinking_period = 0xFFFFFFFF;
    }
      break;
    case 'T':
    {
      uart_transmit_it(USARTx, greeting, sizeof(greeting));
    }
      break;
    case '\r':
    case '\n':
    {
      uart_transmit_it(USARTx, new_line, sizeof(new_line));
    }
      break;
    default:
      break;
  }
}

/* */
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
    blinking_period = BLINKING_FAST;
  }
  else
  {
    /* Turn LED2 on: Transfer error in reception/transmission process */
    blinking_period = 0xFFFFFFFF; // never
    LL_GPIO_SetOutputPin(LED_1_GPIO_Port, LED_1_Pin);
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
