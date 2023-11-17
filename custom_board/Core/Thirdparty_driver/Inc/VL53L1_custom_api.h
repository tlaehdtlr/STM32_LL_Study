/*
 * VL53L1_custom_api.h
 *
 *  Created on: 2023. 11. 10.
 *      Author: Dongsik Sim
 */

#ifndef __VL53L1_CUSTOM_API_H__
#define __VL53L1_CUSTOM_API_H__
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "VL53L1X_api.h"

VL53L1X_ERROR VL53L1X_custom_set_roi(uint16_t dev, 
uint8_t x_centre, uint8_t y_centre, uint8_t x_width, uint8_t y_height);

#endif /* __VL53L1_CUSTOM_API_H__ */