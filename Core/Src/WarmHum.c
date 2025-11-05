/*
 * WarmHum.c
 *
 *  Created on: Nov 3, 2025
 *      Author: yygs3
 */


#include "WarmHum.h"

/* Private variables */
static uint8_t RHI, RHD, TCI, TCD, SUM;

/**
  * @brief  마이크로초 단위 딜레이 함수
  * @param  us: 마이크로초
  * @retval None
  */
static void delay_us(uint32_t us)
{
  uint32_t start = DWT->CYCCNT;
  uint32_t cycles = us * (SystemCoreClock / 1000000);
  while ((DWT->CYCCNT - start) < cycles);
}

/**
  * @brief  GPIO 핀을 출력 모드로 설정
  * @param  None
  * @retval None
  */
static void Set_Pin_Output(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = Warm_Hum_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(Warm_Hum_GPIO_Port, &GPIO_InitStruct);
}

/**
  * @brief  GPIO 핀을 입력 모드로 설정
  * @param  None
  * @retval None
  */
static void Set_Pin_Input(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = Warm_Hum_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Warm_Hum_GPIO_Port, &GPIO_InitStruct);
}

/**
  * @brief  DHT11 센서 초기화
  * @param  None
  * @retval None
  */
void DHT11_Init(void)
{
  /* DWT를 사용한 마이크로초 딜레이를 위한 설정 */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;
}

/**
  * @brief  DHT11 시작 신호 전송
  * @param  None
  * @retval None
  */
void DHT11_Start(void)
{
  Set_Pin_Output();
  HAL_GPIO_WritePin(Warm_Hum_GPIO_Port, Warm_Hum_Pin, GPIO_PIN_RESET);  // Low
  HAL_Delay(18);  // 최소 18ms
  HAL_GPIO_WritePin(Warm_Hum_GPIO_Port, Warm_Hum_Pin, GPIO_PIN_SET);    // High
  delay_us(30);   // 20-40us 대기
  Set_Pin_Input();
}

/**
  * @brief  DHT11 응답 확인
  * @param  None
  * @retval 0: 응답 없음, 1: 응답 있음
  */
uint8_t DHT11_Check_Response(void)
{
  uint8_t response = 0;
  delay_us(40);

  if (!HAL_GPIO_ReadPin(Warm_Hum_GPIO_Port, Warm_Hum_Pin))  // Low 확인
  {
    delay_us(80);
    if (HAL_GPIO_ReadPin(Warm_Hum_GPIO_Port, Warm_Hum_Pin)) { // High 확인
      response = 1;
    }
    else {
      response = 0;
    }
  }

  while (HAL_GPIO_ReadPin(Warm_Hum_GPIO_Port, Warm_Hum_Pin));  // High 종료 대기

  return response;
}

/**
  * @brief  DHT11에서 1바이트 읽기
  * @param  None
  * @retval 읽은 바이트 값
  */
uint8_t DHT11_Read_Byte(void)
{
  uint8_t i;
  uint8_t byte = 0;

  for (i = 0; i < 8; i++)
  {
    while (!HAL_GPIO_ReadPin(Warm_Hum_GPIO_Port, Warm_Hum_Pin));  // Low 종료 대기
    delay_us(40);  // 40us 대기

    if (HAL_GPIO_ReadPin(Warm_Hum_GPIO_Port, Warm_Hum_Pin))  // 여전히 High면 '1'
    {
      byte |= (1 << (7 - i));
    }

    while (HAL_GPIO_ReadPin(Warm_Hum_GPIO_Port, Warm_Hum_Pin));  // High 종료 대기
  }

  return byte;
}

/**
  * @brief  DHT11 데이터 읽기 및 온습도 계산
  * @param  temp: 온도 저장 포인터
  * @param  humi: 습도 저장 포인터
  * @retval 1: 성공, 0: 실패
  */
uint8_t DHT11_Read_Data(float *temp, float *humi)
{
  DHT11_Start();

  if (DHT11_Check_Response())
  {
    RHI = DHT11_Read_Byte();  // 습도 정수부
    RHD = DHT11_Read_Byte();  // 습도 소수부
    TCI = DHT11_Read_Byte();  // 온도 정수부
    TCD = DHT11_Read_Byte();  // 온도 소수부
    SUM = DHT11_Read_Byte();  // 체크섬

    // 체크섬 검증
    if (SUM == (RHI + RHD + TCI + TCD))
    {
      *temp = (float)TCI + (float)TCD / 10.0;
      *humi = (float)RHI + (float)RHD / 10.0;
      return 1;  // 성공
    }
    else
    {
      return 0;  // 체크섬 에러
    }
  }
  else
  {
    return 0;  // 응답 없음
  }
}
