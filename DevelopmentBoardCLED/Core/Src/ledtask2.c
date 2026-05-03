/*
 * ledtask2.c
 *
 *  Created on: May 3, 2026
 *      Author: sohev
 */

#include "ledtask2.h"

// ← remove #ifndef guard entirely

void led_task(void const *argument)  // ← add const
{
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_12);
        osDelay(200);
    }
}
