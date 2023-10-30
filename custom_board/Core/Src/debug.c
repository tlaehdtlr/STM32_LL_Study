#include "debug.h"

/**
 * Check (RCC->CSR)
 * If use a bootloader, this function must execute when bootloader start.
*/
debug_reset_cause_e reset_cause = DEBUG_RESET_UNKNOWN;
char *reset_cause_debug;
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
    printf("\r\n[Reset Cause] : %s \r\n", reset_cause_debug);
}