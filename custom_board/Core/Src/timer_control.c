/**
 * Tim6 
 ** interrupt : 1ms
 ** led blinking, wdt refresh
*/

#include "timer_control.h"


/******************************************************/
/**
 * GPIO 
*/
#define BLINKING_OFFSET 10
#define BLINKING_FAST   (200 + BLINKING_OFFSET)
#define BLINKING_SLOW   (2000 + BLINKING_OFFSET)

volatile uint32_t blinking_period = BLINKING_SLOW;
volatile uint32_t blinking_cnt = BLINKING_OFFSET;

/******************************************************/

/******************************************************/
/**
 * Watchdog timer
*/
#define WDT_TIMEOUT     30  // 30ms

volatile uint16_t wdt_cnt = 0;
/******************************************************/




static void timer_control_check_blinking_led(void);
static void timer_control_wdt(void);


void timer_control_blinking_led_set(bool fast)
{
    if (fast)
    {
        blinking_period = BLINKING_FAST;
    }
    else
    {
        blinking_period = BLINKING_SLOW;
    }
}
uint8_t arr[10] = {1,3,5,7,8,};
static void timer_control_check_blinking_led(void)
{
  if (blinking_cnt - BLINKING_OFFSET > blinking_period)
  {
    blinking_cnt = BLINKING_OFFSET;
    LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
    // uart_transmit_it(USART3, arr, sizeof(arr));
  }
}


static void timer_control_wdt(void)
{
    if (wdt_cnt >= WDT_TIMEOUT)
    {
        wdt_cnt = 0;
        LL_WWDG_SetCounter(WWDG, 120);
    }
}

void timer6_callback(void)
{
  blinking_cnt++;
  wdt_cnt++;
}

void timer_control_idle(void)
{
    timer_control_check_blinking_led();
    timer_control_wdt();
}