/*
 * debug.h
 *
 *  Created on: 2023. 10. 27.
 *      Author: Dongsik Sim
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "main.h"

/**
 * Ref : Drivers\STM32F0xx_HAL_Driver\Inc\stm32f0xx_ll_rcc.h
*/
typedef enum
{
    DEBUG_RESET_UNKNOWN,
    DEBUG_RESET_IWDG,   // independent watchdog
    DEBUG_RESET_LPWR,   // low power
    DEBUG_RESET_OBL,    // Option byte loading
    DEBUG_RESET_PIN,    // HW pin
    DEBUG_RESET_POR,    // power-on/power-down or brownout
    DEBUG_RESET_SFT,    // software
    DEBUG_RESET_WWDG,   // window watchdog
} debug_reset_cause_e;

void debug_reset_cause(void);

#endif /* __DEBUG_H__ */