/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    wwdg.c
  * @brief   This file provides code for the configuration
  *          of the WWDG instances.
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
#include "wwdg.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* WWDG init function */
void MX_WWDG_Init(void)
{

  /* USER CODE BEGIN WWDG_Init 0 */

  /* USER CODE END WWDG_Init 0 */

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_WWDG);

  /* WWDG interrupt Init */
  NVIC_SetPriority(WWDG_IRQn, 0);
  NVIC_EnableIRQ(WWDG_IRQn);

  /* USER CODE BEGIN WWDG_Init 1 */

  /* USER CODE END WWDG_Init 1 */
  LL_WWDG_SetCounter(WWDG, 120);
  LL_WWDG_Enable(WWDG);
  LL_WWDG_SetPrescaler(WWDG, LL_WWDG_PRESCALER_8);
  LL_WWDG_SetWindow(WWDG, 100);
  /* USER CODE BEGIN WWDG_Init 2 */

  /* USER CODE END WWDG_Init 2 */

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
