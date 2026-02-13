/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void display(int num) {
    switch (num) {
        case 0:  HAL_GPIO_WritePin(Num0_GPIO_Port, Num0_Pin, GPIO_PIN_SET); break;
        case 1:  HAL_GPIO_WritePin(Num1_GPIO_Port, Num1_Pin, GPIO_PIN_SET); break;
        case 2:  HAL_GPIO_WritePin(Num2_GPIO_Port, Num2_Pin, GPIO_PIN_SET); break;
        case 3:  HAL_GPIO_WritePin(Num3_GPIO_Port, Num3_Pin, GPIO_PIN_SET); break;
        case 4:  HAL_GPIO_WritePin(Num4_GPIO_Port, Num4_Pin, GPIO_PIN_SET); break;
        case 5:  HAL_GPIO_WritePin(Num5_GPIO_Port, Num5_Pin, GPIO_PIN_SET); break;
        case 6:  HAL_GPIO_WritePin(Num6_GPIO_Port, Num6_Pin, GPIO_PIN_SET); break;
        case 7:  HAL_GPIO_WritePin(Num7_GPIO_Port, Num7_Pin, GPIO_PIN_SET); break;
        case 8:  HAL_GPIO_WritePin(Num8_GPIO_Port, Num8_Pin, GPIO_PIN_SET); break;
        case 9:  HAL_GPIO_WritePin(Num9_GPIO_Port, Num9_Pin, GPIO_PIN_SET); break;
        case 10: HAL_GPIO_WritePin(Num10_GPIO_Port, Num10_Pin, GPIO_PIN_SET); break;
        case 11: HAL_GPIO_WritePin(Num11_GPIO_Port, Num11_Pin, GPIO_PIN_SET); break;
        case 12: HAL_GPIO_WritePin(Num0_GPIO_Port, Num0_Pin, GPIO_PIN_SET); break;
        default: break;
    }
}


void cleardisplay(int num) {
    switch (num) {
        case 0:  HAL_GPIO_WritePin(Num0_GPIO_Port, Num0_Pin, GPIO_PIN_RESET); break;
        case 1:  HAL_GPIO_WritePin(Num1_GPIO_Port, Num1_Pin, GPIO_PIN_RESET); break;
        case 2:  HAL_GPIO_WritePin(Num2_GPIO_Port, Num2_Pin, GPIO_PIN_RESET); break;
        case 3:  HAL_GPIO_WritePin(Num3_GPIO_Port, Num3_Pin, GPIO_PIN_RESET); break;
        case 4:  HAL_GPIO_WritePin(Num4_GPIO_Port, Num4_Pin, GPIO_PIN_RESET); break;
        case 5:  HAL_GPIO_WritePin(Num5_GPIO_Port, Num5_Pin, GPIO_PIN_RESET); break;
        case 6:  HAL_GPIO_WritePin(Num6_GPIO_Port, Num6_Pin, GPIO_PIN_RESET); break;
        case 7:  HAL_GPIO_WritePin(Num7_GPIO_Port, Num7_Pin, GPIO_PIN_RESET); break;
        case 8:  HAL_GPIO_WritePin(Num8_GPIO_Port, Num8_Pin, GPIO_PIN_RESET); break;
        case 9:  HAL_GPIO_WritePin(Num9_GPIO_Port, Num9_Pin, GPIO_PIN_RESET); break;
        case 10: HAL_GPIO_WritePin(Num10_GPIO_Port, Num10_Pin, GPIO_PIN_RESET); break;
        case 11: HAL_GPIO_WritePin(Num11_GPIO_Port, Num11_Pin, GPIO_PIN_RESET); break;
        case 12: HAL_GPIO_WritePin(Num0_GPIO_Port, Num0_Pin, GPIO_PIN_RESET); break;
        default: break;
    }
}

void displayall(void) {
    HAL_GPIO_WritePin(Num0_GPIO_Port, Num0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Num1_GPIO_Port, Num1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Num2_GPIO_Port, Num2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Num3_GPIO_Port, Num3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Num4_GPIO_Port, Num4_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Num5_GPIO_Port, Num5_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Num6_GPIO_Port, Num6_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Num7_GPIO_Port, Num7_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Num8_GPIO_Port, Num8_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Num9_GPIO_Port, Num9_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Num10_GPIO_Port, Num10_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Num11_GPIO_Port, Num11_Pin, GPIO_PIN_SET);
}

void clearallClock(void) {
    HAL_GPIO_WritePin(Num0_GPIO_Port, Num0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Num1_GPIO_Port, Num1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Num2_GPIO_Port, Num2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Num3_GPIO_Port, Num3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Num4_GPIO_Port, Num4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Num5_GPIO_Port, Num5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Num6_GPIO_Port, Num6_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Num7_GPIO_Port, Num7_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Num8_GPIO_Port, Num8_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Num9_GPIO_Port, Num9_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Num10_GPIO_Port, Num10_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Num11_GPIO_Port, Num11_Pin, GPIO_PIN_RESET);
}

void setNumberOnClock(int num){
	display(num);
}

void clearNumberOnClock(int num)
{
	cleardisplay(num);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int state = 0;
  while (1)
  {
	  clearallClock();

	     if(state == 0) {
	         setNumberOnClock(5);   // hiện số 5
	         state = 1;             // đổi trạng thái
	     } else {
	         clearNumberOnClock(5); // tắt số 5
	         state = 0;
	     }

	     HAL_Delay(1000);  // chỉ dùng 1 delay
  }
  /* USER CODE END WHILE */
}
/* <-- ĐÃ THÊM DẤU ĐÓNG } KẾT THÚC main() */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, Num0_Pin|Num1_Pin|Num2_Pin|Num3_Pin
                          |Num4_Pin|Num5_Pin|Num6_Pin|Num7_Pin
                          |Num8_Pin|Num9_Pin|Num10_Pin|Num11_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = Num0_Pin|Num1_Pin|Num2_Pin|Num3_Pin
                          |Num4_Pin|Num5_Pin|Num6_Pin|Num7_Pin
                          |Num8_Pin|Num9_Pin|Num10_Pin|Num11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
