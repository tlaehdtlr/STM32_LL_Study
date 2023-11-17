/*
 * tof.h
 *
 *  Created on: 2023. 11. 08.
 *      Author: Dongsik Sim
 */

#ifndef __TOF_H__
#define __TOF_H__
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"

void tof_start_measurement(void);
void tof_start(bool state);
void tof_get_config(void);
void tof_init(void);
void tof_receive_data(void);
#endif /* __TOF_H__ */