/* uart_it_motor.h */
#ifndef __UART_IT_MOTOR_H
#define __UART_IT_MOTOR_H

#include "stm32f4xx_hal.h"

/* Defines */
#define SERVO_0_DEG    500   // 0도 (0.5ms)
#define SERVO_90_DEG   1500  // 90도 (1.5ms)
#define SERVO_180_DEG  2500  // 180도 (2.5ms)

/* External variables */
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart3;
extern uint8_t rx_data;
extern volatile uint8_t servo_flag;
extern volatile uint32_t servo_start_time;

/* Function prototypes */
void Servo_SetAngle(uint16_t angle);
void Motor_Init(void);
void Motor_Control(void);


#endif /* __UART_IT_MOTOR_H */
