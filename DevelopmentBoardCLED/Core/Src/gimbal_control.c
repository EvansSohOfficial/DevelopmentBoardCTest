/*
 * gimbal_control.c
 *
 *  Created on: May 3, 2026
 *      Author: sohev
 */

#include "gimbal_control.h"

// ── PID parameters ──────────────────────────────────
// Outer loop: position → speed setpoint
#define YAW_POSITION_KP        15.0f
#define YAW_POSITION_KI         0.0f
#define YAW_POSITION_KD         0.0f
#define YAW_POSITION_MAX_OUT  300.0f
#define YAW_POSITION_MAX_I      0.0f

// Inner loop: speed → voltage output
#define YAW_SPEED_KP         1800.0f
#define YAW_SPEED_KI           20.0f
#define YAW_SPEED_KD            0.0f
#define YAW_SPEED_MAX_OUT   25000.0f
#define YAW_SPEED_MAX_I      5000.0f
// ────────────────────────────────────────────────────

static PID_t yaw_pos_pid;
static PID_t yaw_spd_pid;

// Target angle 0-8191 (4096 = 180 degrees)
static uint16_t target_angle = 4096;

static void PID_Init(PID_t *pid,
                     float kp, float ki, float kd,
                     float max_out, float max_i)
{
    pid->kp           = kp;
    pid->ki           = ki;
    pid->kd           = kd;
    pid->max_output   = max_out;
    pid->max_integral = max_i;
    pid->error        = 0.0f;
    pid->last_error   = 0.0f;
    pid->integral     = 0.0f;
    pid->output       = 0.0f;
}

static float PID_Calc(PID_t *pid, float ref, float measure)
{
    pid->last_error = pid->error;
    pid->error      = ref - measure;

    pid->integral  += pid->error;

    // Clamp integral
    if (pid->integral >  pid->max_integral) pid->integral =  pid->max_integral;
    if (pid->integral < -pid->max_integral) pid->integral = -pid->max_integral;

    pid->output = pid->kp * pid->error
                + pid->ki * pid->integral
                + pid->kd * (pid->error - pid->last_error);

    // Clamp output
    if (pid->output >  pid->max_output) pid->output =  pid->max_output;
    if (pid->output < -pid->max_output) pid->output = -pid->max_output;

    return pid->output;
}

// Handle encoder wrap-around (0-8191 = full circle)
static float angle_error(uint16_t target, uint16_t current)
{
    float err = (float)target - (float)current;
    if (err >  4096.0f) err -= 8192.0f;
    if (err < -4096.0f) err += 8192.0f;
    return err;
}

void gimbal_control(void const *argument)
{
    // Init both PID loops
    PID_Init(&yaw_pos_pid,
             YAW_POSITION_KP, YAW_POSITION_KI, YAW_POSITION_KD,
             YAW_POSITION_MAX_OUT, YAW_POSITION_MAX_I);

    PID_Init(&yaw_spd_pid,
             YAW_SPEED_KP, YAW_SPEED_KI, YAW_SPEED_KD,
             YAW_SPEED_MAX_OUT, YAW_SPEED_MAX_I);

    // Wait for CAN and motor to be ready
    osDelay(100);
    CAN1_Init_Filters();

    while(1)
    {
        // Poll for incoming messages instead of using interrupt
        if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0)
        {
            CAN_RxHeaderTypeDef rx_header;
            uint8_t rx_data[8];
            HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data);

            // Store directly
            gm6020_feedback[0].angle = (rx_data[0] << 8) | rx_data[1];
            gm6020_feedback[0].rpm   = (rx_data[2] << 8) | rx_data[3];

            // LED ON if we got something
            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_SET);
        }
        else
        {
            // Still no messages
            HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_12);
        }

        osDelay(10);
    }
}
