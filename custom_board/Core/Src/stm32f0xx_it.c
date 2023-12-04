/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern CAN_HandleTypeDef hcan;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles Window watchdog interrupt.
  */
void WWDG_IRQHandler(void)
{
  /* USER CODE BEGIN WWDG_IRQn 0 */

  /* USER CODE END WWDG_IRQn 0 */
  /* USER CODE BEGIN WWDG_IRQn 1 */

  /* USER CODE END WWDG_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global and DAC channel underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  if(LL_TIM_IsActiveFlag_UPDATE(TIM6) == 1)
  {
    /* Clear the update interrupt flag*/
    LL_TIM_ClearFlag_UPDATE(TIM6);
  }
  timer6_callback();
  
  /* USER CODE END TIM6_DAC_IRQn 0 */

  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles I2C1 event global interrupt / I2C1 wake-up interrupt through EXTI line 23.
  */
void I2C1_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_IRQn 0 */

  // transmit

#if 0
  // /* Check NACK flag value in ISR register */
  
  if(LL_I2C_IsActiveFlag_NACK(I2C1))
  {
    /* End of Transfer */
    LL_I2C_ClearFlag_NACK(I2C1);
    printf("NACK \r\n");
  }
  /* Check TXIS flag value in ISR register */
  if(LL_I2C_IsActiveFlag_TXIS(I2C1))
  {
    // set_tx_ready(true);
    printf("TXIS \r\n");
  }
  /* Check STOP flag value in ISR register */
  // else if(LL_I2C_IsActiveFlag_STOP(I2C1))
  // {
  //   /* Clear STOP flag value in ISR register */
  //   LL_I2C_ClearFlag_STOP(I2C1);
  //   /* Check TXE flag value in ISR register */
  // }

  if(!LL_I2C_IsActiveFlag_TXE(I2C1))
  {
    /* Flush the TXDR register */
    LL_I2C_ClearFlag_TXE(I2C1);
    printf("TXE \r\n");
  }
#endif

// receive

  /* Check RXNE flag value in ISR register */
  if(LL_I2C_IsActiveFlag_RXNE(I2C1))
  {
    /* Call function Master Reception Callback */
    // Master_Reception_Callback();
    // printf("RXNE \r\n");
  }
  /* Check STOP flag value in ISR register */
  if(LL_I2C_IsActiveFlag_STOP(I2C1))
  {
    /* End of Transfer */
    LL_I2C_ClearFlag_STOP(I2C1);

    /* Call function Master Complete Callback */
    // Master_Complete_Callback();
    // printf("STOP \r\n");
  }

  if (LL_I2C_IsActiveFlag_TC(I2C1))
  {
    
  }
  // else
  // {
  //   /* Call Error function */
  //   // Error_Handler();
  // }


  /* USER CODE END I2C1_IRQn 0 */

  /* USER CODE BEGIN I2C1_IRQn 1 */

  /* USER CODE END I2C1_IRQn 1 */
}

/**
  * @brief This function handles USART3 to USART8 global interrupts / USART3 wake-up interrupt through EXTI line 28.
  */
void USART3_8_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_8_IRQn 0 */
  if (LL_USART_IsActiveFlag_RXNE(USART3) && LL_USART_IsEnabledIT_RXNE(USART3))
  {
    uart_receive_callback(USART3);
  }

  if(LL_USART_IsEnabledIT_TXE(USART3) && LL_USART_IsActiveFlag_TXE(USART3))
  {
    /* TXE flag will be automatically cleared when writing new data in TDR register */

    /* Call function in charge of handling empty DR => will lead to transmission of next character */
    // uart_transmit_callback(USART3);
    uart_txe_callback(USART3);
  }

  if(LL_USART_IsEnabledIT_TC(USART3) && LL_USART_IsActiveFlag_TC(USART3))
  {
    /* Clear TC flag */
    LL_USART_ClearFlag_TC(USART3);
    uart_tc_callback(USART3);
  }

  /* USER CODE END USART3_8_IRQn 0 */

  /* USER CODE BEGIN USART3_8_IRQn 1 */
  // if(LL_USART_IsEnabledIT_ERROR(USART3) && LL_USART_IsActiveFlag_NE(USART3))
  if(LL_USART_IsEnabledIT_ERROR(USART3) && LL_USART_IsActiveFlag_NE(USART3))
  {
    /* Call Error function */
    // UART_Error_Callback();
  }
  /* USER CODE END USART3_8_IRQn 1 */
}

/**
  * @brief This function handles HDMI-CEC and CAN global interrupts / HDMI-CEC wake-up interrupt through EXTI line 27.
  */
void CEC_CAN_IRQHandler(void)
{
  /* USER CODE BEGIN CEC_CAN_IRQn 0 */

  /* USER CODE END CEC_CAN_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan);
  /* USER CODE BEGIN CEC_CAN_IRQn 1 */

  /* USER CODE END CEC_CAN_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
