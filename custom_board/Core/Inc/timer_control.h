/*
 * timer_control.h
 *
 *  Created on: 2023. 10. 18.
 *      Author: Dongsik Sim
 */

#ifndef __TIMER_CONTROL_H__
#define __TIMER_CONTROL_H__

#include <stdbool.h>
#include <stdio.h>
#include "main.h"
#include "usart.h"

void timer_control_blinking_led_set(bool fast);
void set_delay_ms(uint16_t delay);
void set_retry_func(void* func, uint16_t delay);
void timer6_callback(void);
void timer_control_idle(void);
#endif /* __TIMER_CONTROL_H__ */
