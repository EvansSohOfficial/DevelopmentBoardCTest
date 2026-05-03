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

// PID structure
typedef struct {
    float kp, ki, kd;
    float error, last_error;
    float integral;
    float output;
    float max_output;
    float max_integral;
} PID_t;

void gimbal_control(void const *argument);

#endif /* INC_GIMBAL_CONTROL_H_ */
