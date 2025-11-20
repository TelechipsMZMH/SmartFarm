/*
 * soil_hum.c
 *
 *  Created on: Nov 10, 2025
 *      Author: user
 */

#include "main.h"
#include "soil_hum.h"




#define Samples 10		// sampling count
#define ADC_DRY	4000	//MAX DRY
#define ADC_WET 1000	// 측정값
#define printf	Uart3_Printf

void Soil_Start(void)
{
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, 100);
}

/**
  * @brief  Soil (릴레이 모듈 제어)
  * @param  None
  * @retval None
  */

uint8_t SoilVal_Avg(void)
{
    uint16_t soil_val = 0;
    uint16_t avg_adc = 0;

    for (int i = 0; i < Samples; i++)
    {
    	Soil_Start();
        soil_val += HAL_ADC_GetValue(&hadc2);
        HAL_Delay(100);
    }

    avg_adc = soil_val / Samples;
    return Get_Percentage(avg_adc);
}



/**
  * @brief  Set Percentage
  * @param  soil_adc
  * @retval soil_moisture Percentage
  */

uint8_t Get_Percentage(uint16_t soil_adc)
{
    uint16_t soil_moisture;

    // Dry??
    if (soil_adc >= ADC_DRY)
        soil_moisture = 0;
    // Too many Water
    else if (soil_adc <= ADC_WET)
        soil_moisture = 100;
    else
        soil_moisture = (ADC_DRY - soil_adc) * 100 / (ADC_DRY - ADC_WET);

    return (uint8_t)soil_moisture;
}





