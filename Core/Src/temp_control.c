/*
 * temp_control.c
 *
 *  Created on: Nov 3, 2025
 *      Author: yygs3
 */


#include "temp_control.h"

/* Private variables */
static uint8_t fan_state = 0;      // 0: OFF, 1: ON
static uint8_t heater_state = 0;   // 0: OFF, 1: ON

/**
  * @brief  온도 제어 모듈 초기화
  * @param  None
  * @retval None
  */
void TempControl_Init(void)
{
  // 초기 상태: 모든 장치 OFF
  TempControl_FanOff();
  TempControl_HeaterOff();
  fan_state = 0;
  heater_state = 0;
}

/**
  * @brief  온도에 따른 팬 및 발열패드 제어
  * @param  temperature: 현재 온도 (섭씨)
  * @retval None
  */
void TempControl_Update(float temperature)
{
  // 팬 제어: 30도 이상 시 ON, 이하 시 OFF
  if (temperature >= TEMP_HIGH_THRESHOLD)
  {
    if (!fan_state)
    {
      TempControl_FanOn();
      fan_state = 1;
    }
  }
  else
  {
    if (fan_state)
    {
      TempControl_FanOff();
      fan_state = 0;
    }
  }

  // 발열패드 제어: 10도 이하 시 ON, 초과 시 OFF
  if (temperature <= TEMP_LOW_THRESHOLD)
  {
    if (!heater_state)
    {
      TempControl_HeaterOn();
      heater_state = 1;
    }
  }
  else
  {
    if (heater_state)
    {
      TempControl_HeaterOff();
      heater_state = 0;
    }
  }
}

/**
  * @brief  팬 ON (릴레이 모듈 제어)
  * @param  None
  * @retval None
  */
void TempControl_FanOn(void)
{
  HAL_GPIO_WritePin(FAN_Moter_GPIO_Port, FAN_Moter_Pin, GPIO_PIN_SET);
  DP_set_fan_state(1);
}

/**
  * @brief  팬 OFF (릴레이 모듈 제어)
  * @param  None
  * @retval None
  */
void TempControl_FanOff(void)
{
  HAL_GPIO_WritePin(FAN_Moter_GPIO_Port, FAN_Moter_Pin, GPIO_PIN_RESET);
  DP_set_fan_state(0);
}

/**
  * @brief  발열패드 ON (릴레이 모듈 제어)
  * @param  None
  * @retval None
  */
void TempControl_HeaterOn(void)
{
  HAL_GPIO_WritePin(HeatingPad_GPIO_Port, HeatingPad_Pin, GPIO_PIN_SET);
  DP_set_heat_state(1);
}

/**
  * @brief  발열패드 OFF (릴레이 모듈 제어)
  * @param  None
  * @retval None
  */
void TempControl_HeaterOff(void)
{
  HAL_GPIO_WritePin(HeatingPad_GPIO_Port, HeatingPad_Pin, GPIO_PIN_RESET);
  DP_set_heat_state(0);

}
