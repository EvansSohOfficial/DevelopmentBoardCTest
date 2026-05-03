/*
 * ledtask2.c
 *
 *  Created on: May 3, 2026
 *      Author: sohev
 */

#ifndef SRC_LEDTASK2_C_
#define SRC_LEDTASK2_C_


#include "ledtask2.h"

void led_task(void *argument)
{
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_12);
        osDelay(500);
    }
}

#endif /* SRC_LEDTASK2_C_ */
