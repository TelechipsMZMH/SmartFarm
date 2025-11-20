/*
 * temp_control.h
 *
 *  Created on: Nov 3, 2025
 *      Author: yygs3
 */

#ifndef __TEMP_CONTROL_H
#define __TEMP_CONTROL_H

#include "main.h"

/* Temperature thresholds */
#define TEMP_HIGH_THRESHOLD  26.0f  // 팬 작동 온도 (섭씨)
#define TEMP_LOW_THRESHOLD   10.0f  // 발열패드 작동 온도 (섭씨)

/* Function prototypes */
void TempControl_Init(void);
void TempControl_Update(float temperature);
void TempControl_FanOn(void);
void TempControl_FanOff(void);
void TempControl_HeaterOn(void);
void TempControl_HeaterOff(void);

#endif /* __TEMP_CONTROL_H */
