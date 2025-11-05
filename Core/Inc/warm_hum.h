/*
 * warm_hum.h
 *
 *  Created on: Nov 5, 2025
 *      Author: yygs3
 */

#ifndef INC_WARM_HUM_H_
#define INC_WARM_HUM_H_

#include "main.h"

/* Function prototypes */
void DHT11_Init(void);
void DHT11_Start(void);
uint8_t DHT11_Check_Response(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(float *temp, float *humi);

#endif /* INC_WARM_HUM_H_ */
