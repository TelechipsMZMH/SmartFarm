/* uart_it_motor.c */
#include "uart_it_motor.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/* Variables */
uint8_t rx_data;
volatile uint8_t servo_flag = 0;
volatile uint32_t servo_start_time = 0;

void Servo_SetAngle(uint16_t angle)
{
    // PWM 펄스로 변환
    uint16_t pulse = SERVO_0_DEG + (angle * (SERVO_180_DEG - SERVO_0_DEG) / 180);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pulse);
}

void Motor_Init(void)
{
    // PWM 시작
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    Servo_SetAngle(90);  // 초기 위치 90도로 설정

    // UART 인터럽트 수신 시작
    HAL_UART_Receive_IT(&huart3, &rx_data, 1);
}

void Motor_Control(void)
{
    // 서보모터 제어 (인터럽트 플래그 확인)
    if(servo_flag == 1){
//        if(HAL_GetTick() - servo_start_time < 1000){
//            Servo_SetAngle(0);  // 0도 회전
//        }
//        else{
//            Servo_SetAngle(90);   // 정지
//            servo_flag = 0;      // 플래그 초기화
//        }
    	Servo_SetAngle(0);    // Q 입력 → 반시계 90° (0도)
        servo_flag = 0;
    }
    else if(servo_flag == 2){
//        if(HAL_GetTick() - servo_start_time < 1000){
//            Servo_SetAngle(180);  // 180도 회전
//        }
//        else{
//            Servo_SetAngle(90);   // 정지
//            servo_flag = 0;      // 플래그 초기화
//        }
    	Servo_SetAngle(180);  // R 입력 → 시계 90° (180도)
        servo_flag = 0;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART3)
    {
        if(rx_data == 'q' || rx_data == 'Q')
        {
            servo_flag = 1;
            Uart3_Printf("Servo rotate\n");
        }
        if(rx_data == 'r' || rx_data == 'R')
        {
             servo_flag = 2;
             Uart3_Printf("Servo rotate\n");
        }
        Motor_Control();
        HAL_UART_Receive_IT(&huart3, &rx_data, 1);
    }
}
