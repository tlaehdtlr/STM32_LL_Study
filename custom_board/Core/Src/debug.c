#include "debug.h"

#define DEBUG_ERROR_OCCUR       0x77

debug_reset_cause_e reset_cause = DEBUG_RESET_UNKNOWN;
char *reset_cause_debug;

__attribute__((section(".noinit"))) volatile uint8_t debug_log_empty;
__attribute__((section(".noinit"))) char debug_error_log[32];
__attribute__((section(".noinit"))) volatile uint32_t debug_error_line;

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
    if (LL_RCC_IsActiveFlag_LPWRRST())
    {
        reset_cause = DEBUG_RESET_LPWR;
        reset_cause_debug = "DEBUG_RESET_LPWR";
    }
    if (LL_RCC_IsActiveFlag_OBLRST())
    {
        reset_cause = DEBUG_RESET_OBL;
        reset_cause_debug = "DEBUG_RESET_OBL";
    }
    if (LL_RCC_IsActiveFlag_PINRST())
    {
        reset_cause = DEBUG_RESET_PIN;
        reset_cause_debug = "DEBUG_RESET_PIN";
    }
    if (LL_RCC_IsActiveFlag_PORRST())
    {
        reset_cause = DEBUG_RESET_POR;
        reset_cause_debug = "DEBUG_RESET_POR";
    }
    if (LL_RCC_IsActiveFlag_SFTRST())
    {
        reset_cause = DEBUG_RESET_SFT;
        reset_cause_debug = "DEBUG_RESET_SFT";
    }
    if (LL_RCC_IsActiveFlag_WWDGRST())
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
        debug_log_empty = 0x00;
    }
    else
    {
        debug_show_log();
    }

    printf("\r\n[Reset Cause] : %s \r\n", reset_cause_debug);
}

void debug_log_error(char * file, uint32_t line)
{
    debug_log_empty = DEBUG_ERROR_OCCUR;
    strcpy(debug_error_log, file);
    debug_error_line = line;
}

void debug_show_log(void)
{
    if (debug_log_empty == DEBUG_ERROR_OCCUR)
    {
        printf("[Debug log]: file %c on line %ld\r\n", *debug_error_log, debug_error_line);
    }
    else
    {
        printf("[Debug log]: nothing \r\n");
    }
}