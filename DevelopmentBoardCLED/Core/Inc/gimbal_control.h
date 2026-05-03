/*
 * gimbal_control.h
 *
 *  Created on: May 3, 2026
 *      Author: sohev
 */

#ifndef INC_GIMBAL_CONTROL_H_
#define INC_GIMBAL_CONTROL_H_

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "can_communication.h"

void gimbal_control(void *argument);

#endif
