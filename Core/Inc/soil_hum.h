/*
 * soil_hum.h
 *
 *  Created on: Nov 17, 2025
 *      Author: user
 */

#ifndef INC_SOIL_HUM_H_
#define INC_SOIL_HUM_H_


void Soil_Init(ADC_HandleTypeDef *hadc);
void Soil_Start();
uint8_t SoilVal_Avg(void);
uint8_t Get_Percentage(uint16_t soil_adc);
void Soil_Moisture_Action(void);

#endif /* INC_SOIL_HUM_H_ */
