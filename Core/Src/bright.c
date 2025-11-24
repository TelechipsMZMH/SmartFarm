/* bright.c */
#include "bright.h"
#include "uart_it_motor.h"

/* Variables */
uint32_t adc_value = 0;
float voltage = 0;

void Bright_Control(void)
{
    // ADC 조도센서 읽기
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    adc_value = HAL_ADC_GetValue(&hadc1);
    voltage = (adc_value / 4095.0f) * 3.3f;
    Uart3_Printf("bright: %d\n", adc_value);

    // 조도에 따른 LED 제어
    if(adc_value < THRESHOLD){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 1);
    }
    else{
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 0);
    }
}
