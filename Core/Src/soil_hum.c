/*
 * soil_hum.c
 *
 *  Created on: Nov 10, 2025
 *      Author: user
 */

#include "main.h"
#include "soil_hum.h"


#define Samples 10		// ADC sample count
#define ADC_DRY	4000	// ADC value corresponding to completely dry soil
#define ADC_WET 1000	// ADC value corresponding to fully wet soil
#define printf	Uart3_Printf

ADC_HandleTypeDef *adc_channel = NULL;

/* Private variables */
static uint16_t soil_val = 0;
static uint16_t avg_adc = 0;
static uint16_t soil_moisture = 0;
static uint16_t boundary = 0;

/**
  * @brief  Soil (릴레이 모듈 제어)
  * @param  None
  * @retval None
  */

void Soil_Init(ADC_HandleTypeDef *hadc)
{
    soil_val = 0;
    avg_adc = 0;
    soil_moisture = 0;
    adc_channel = hadc;
}

/**
  * @brief  Soil (릴레이 모듈 제어)
  * @param  None
  * @retval None
  */

void Soil_Start()
{
	HAL_ADC_Start(adc_channel);
	HAL_ADC_PollForConversion(adc_channel, 100);
}

/**
  * @brief  Soil (릴레이 모듈 제어)
  * @param  None
  * @retval None
  */

uint8_t SoilVal_Avg()
{

    for (int i = 0; i < Samples; i++)
    {
    	Soil_Start();
        soil_val += HAL_ADC_GetValue(adc_channel);
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

    // Dry??
    if (soil_adc >= ADC_DRY)
    {
        soil_moisture = 0;
    }

    // Too many Water
    else if (soil_adc <= ADC_WET)
    {
    	soil_moisture = 100;
    }

    // Check soil moisture
    else
    {
        soil_moisture = (ADC_DRY - soil_adc) * 100 / (ADC_DRY - ADC_WET);
    }

    return (uint8_t)soil_moisture;
}

// To Use Main Loop
void Soil_Moisture_Action()
{
	// set Moisture Data
	boundary = SoilVal_Avg();

	// Water On
	// moisture Percentage 10% down
	if(boundary <= 10)
	{
	  		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_3);
	  		HAL_Delay(500);
	}

	// Another Case
	else{ }
	// print Moisture Percentage
	printf("Moisture : %d%%\r\n",boundary);

}




