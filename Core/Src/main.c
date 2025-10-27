/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint8_t RHI, RHD, TCI, TCD, SUM;
float temperature = 0;
float humidity = 0;
char msg[200];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
void DHT11_Start(void);
uint8_t DHT11_Check_Response(void);
uint8_t DHT11_Read_Byte(void);
void DHT11_Read_Data(void);
void Set_Pin_Output(void);
void Set_Pin_Input(void);
void delay_us(uint32_t us);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  snprintf(msg, sizeof(msg), "\r\n===== DHT11 Temperature Sensor =====\r\n");
  HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), 1000);
  HAL_Delay(2000);  // DHT11 초기화 대기
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    DHT11_Read_Data();

    snprintf(msg, sizeof(msg), "Temperature: %.1f C\r\n", temperature);
    HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), 1000);

    snprintf(msg, sizeof(msg), "Humidity: %.1f %%\r\n\r\n", humidity);
    HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), 1000);

    HAL_Delay(3000);  // 3초마다 측정
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Warm_Hum_GPIO_Port, Warm_Hum_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : Warm_Hum_Pin */
  GPIO_InitStruct.Pin = Warm_Hum_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(Warm_Hum_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* DWT를 사용한 마이크로초 딜레이를 위한 설정 */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief  마이크로초 단위 딜레이 함수
  * @param  us: 마이크로초
  * @retval None
  */
void delay_us(uint32_t us)
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
void Set_Pin_Output(void)
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
void Set_Pin_Input(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = Warm_Hum_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Warm_Hum_GPIO_Port, &GPIO_InitStruct);
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
    if (HAL_GPIO_ReadPin(Warm_Hum_GPIO_Port, Warm_Hum_Pin))  // High 확인
      response = 1;
    else
      response = 0;
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
  uint8_t i, byte = 0;

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
  * @param  None
  * @retval None
  */
void DHT11_Read_Data(void)
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
      temperature = (float)TCI + (float)TCD / 10.0;
      humidity = (float)RHI + (float)RHD / 10.0;
    }
    else
    {
      snprintf(msg, sizeof(msg), "Checksum Error!\r\n");
      HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), 1000);
    }
  }
  else
  {
    snprintf(msg, sizeof(msg), "DHT11 No Response!\r\n");
    HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), 1000);
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
