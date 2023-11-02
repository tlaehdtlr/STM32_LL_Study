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

#define GET_MACRO( _0, _1, NAME, ... )  NAME
#define Error_Handler(...)          GET_MACRO( _0, ##__VA_ARGS__, Error_Handler1, Error_Handler0 ) ()
#define Error_Handler0()            debug_error_handler((uint8_t*)__FILE__, __func__, __LINE__ )
// #define Error_Handler1(unused)      printf("Hello ERR1 \r\n");

#define DEBUG_COLOR_RED      "\x1b[31m"
#define DEBUG_COLOR_GREEN    "\x1b[32m"
#define DEBUG_COLOR_YELLOW   "\x1b[33m"
#define DEBUG_COLOR_BLUE     "\x1b[34m"
#define DEBUG_COLOR_MAGENTA  "\x1b[35m"
#define DEBUG_COLOR_CYAN     "\x1b[36m"
#define DEBUG_COLOR_RESET    "\x1b[0m"

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

typedef struct
{
    uint8_t err_log_occur;
    char err_log_file[32];
    char err_log_func[32];
    uint32_t err_log_line;
} debug_err_log_t;


void debug_reset_cause(void);
void debug_log_error(char * file, const char * func, uint32_t line);
void debug_show_log(void);
void debug_error_handler(uint8_t * file, const char * func, uint32_t line);

#endif /* __DEBUG_H__ */