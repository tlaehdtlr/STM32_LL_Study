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

volatile uint16_t delay_ms = 0;
volatile uint16_t delay_cnt = 0;


void (*retry_func)(void);
volatile uint16_t retry_tim = 0;
volatile uint16_t retry_cnt = 0;

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

static void timer_control_check_blinking_led(void)
{
  if (blinking_cnt - BLINKING_OFFSET > blinking_period)
  {
    blinking_cnt = BLINKING_OFFSET;
    LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
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
  delay_cnt++;
  retry_cnt++;
}

void set_delay_ms(uint16_t delay)
{
  delay_ms = delay;
  delay_cnt = 0;
  while (delay_cnt!=delay_ms);
}

void set_retry_func(void* func, uint16_t delay)
{
  retry_func = func;
  retry_tim = delay;
}

void check_retry_func(void)
{
  if (retry_tim)
  {
    if (retry_cnt >= retry_tim)
    {
      retry_cnt = 0;
      retry_func();
    }
  }
}

void timer_control_idle(void)
{
    timer_control_check_blinking_led();
    // timer_control_wdt();
    check_retry_func();
}