/* bright.h */
#ifndef __BRIGHT_H
#define __BRIGHT_H

#include "stm32f4xx_hal.h"

/* Defines */
#define THRESHOLD 2000  // 조도 임계값

/* External variables */
extern ADC_HandleTypeDef hadc1;
extern uint32_t adc_value;
extern float voltage;

/* Function prototypes */
void Bright_Control(void);

#endif /* __BRIGHT_H */
