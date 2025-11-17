/*
 * soil_hum.h
 *
 *  Created on: Nov 17, 2025
 *      Author: user
 */

#ifndef INC_SOIL_HUM_H_
#define INC_SOIL_HUM_H_

extern ADC_HandleTypeDef hadc1;
void Soil_Start(void);
uint8_t SoilVal_Avg(void);
uint8_t Get_Percentage(uint16_t soil_adc);


#endif /* INC_SOIL_HUM_H_ */
