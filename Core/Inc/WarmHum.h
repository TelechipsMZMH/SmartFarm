/*
 * WarmHum.h
 *
 *  Created on: Nov 3, 2025
 *      Author: yygs3
 */

#ifndef __WARMHUM_H
#define __WARMHUM_H

#include "main.h"

/* Function prototypes */
void DHT11_Init(void);
void DHT11_Start(void);
uint8_t DHT11_Check_Response(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(float *temp, float *humi);

#endif /* __DHT11_H */
