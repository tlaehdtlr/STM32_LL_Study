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

void timer6_callback(void);
void timer_control_idle(void);
#endif /* __TIMER_CONTROL_H__ */
