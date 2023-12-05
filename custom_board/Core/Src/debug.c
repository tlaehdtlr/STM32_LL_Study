#include "debug.h"
#include "usart.h"

#define DEBUG_ERROR_OCCUR       0x77
__attribute__((section(".noinit"))) debug_err_log_t error_log;

debug_reset_cause_e reset_cause = DEBUG_RESET_UNKNOWN;
char *reset_cause_debug;


/**
 * Check (RCC->CSR)
 * If use a bootloader, this function must execute when bootloader start.
*/
void debug_reset_cause(void)
{
    /*
    ifdef (APPLICATION)
    */

    if (LL_RCC_IsActiveFlag_IWDGRST())
    {
        reset_cause = DEBUG_RESET_IWDG;
        reset_cause_debug = "DEBUG_RESET_IWDG";
    }
    else if (LL_RCC_IsActiveFlag_LPWRRST())
    {
        reset_cause = DEBUG_RESET_LPWR;
        reset_cause_debug = "DEBUG_RESET_LPWR";
    }
    else if (LL_RCC_IsActiveFlag_OBLRST())
    {
        reset_cause = DEBUG_RESET_OBL;
        reset_cause_debug = "DEBUG_RESET_OBL";
    }
    else if (LL_RCC_IsActiveFlag_PINRST())
    {
        reset_cause = DEBUG_RESET_PIN;
        reset_cause_debug = "DEBUG_RESET_PIN";
    }
    else if (LL_RCC_IsActiveFlag_PORRST())
    {
        reset_cause = DEBUG_RESET_POR;
        reset_cause_debug = "DEBUG_RESET_POR";
    }
    else if (LL_RCC_IsActiveFlag_SFTRST())
    {
        reset_cause = DEBUG_RESET_SFT;
        reset_cause_debug = "DEBUG_RESET_SFT";
    }
    else if (LL_RCC_IsActiveFlag_WWDGRST())
    {
        reset_cause = DEBUG_RESET_WWDG;
        reset_cause_debug = "DEBUG_RESET_WWDG";
    }

    /* clear reset flag */
    LL_RCC_ClearResetFlags();
    if (!reset_cause)
    {
        reset_cause_debug = "DEBUG_RESET_UNKNOWN";
        LL_GPIO_SetOutputPin(LED_3_GPIO_Port, LED_3_Pin);
    }

    if (reset_cause != DEBUG_RESET_IWDG && reset_cause != DEBUG_RESET_WWDG)
    {
        error_log.err_log_occur = 0x00;
    }

    printf("\r\n" DEBUG_COLOR_GREEN "[Reset Cause] : %s" DEBUG_COLOR_RESET "\r\n", reset_cause_debug);
    debug_show_log();
}

void debug_log_error(char * file, const char * func, uint32_t line)
{
    error_log.err_log_occur = DEBUG_ERROR_OCCUR;
    strcpy(error_log.err_log_file, file);
    strcpy(error_log.err_log_func, func);
    error_log.err_log_line = line;
}

void debug_show_log(void)
{
    if (error_log.err_log_occur == DEBUG_ERROR_OCCUR)
    {
        printf(DEBUG_COLOR_RED "[Debug log]: %s %ld line , %s\r\n" DEBUG_COLOR_RESET, error_log.err_log_file, error_log.err_log_line, error_log.err_log_func);
        error_log.err_log_occur = 0;
    }
}

void debug_error_handler(uint8_t * file, const char * func, uint32_t line)
{
    debug_log_error((char*)file, (const char*)func, line);
    while (1)
    {
        uart_idle();
    }
}