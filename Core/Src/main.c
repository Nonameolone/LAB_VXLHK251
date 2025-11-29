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
#include "string.h"
#include "stdio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// STATE cho command parser
typedef enum {
  IDLE = 0,
  START_PROCESS,
  RECEIVE_O,
  RECEIVE_R,
  RECEIVE_S,
  RECEIVE_T,
  RECEIVE_K,
  START_SEND_DATA_SIGNAL,
  STOP_SEND_DATA_SIGNAL
} CommandParserState_t;

// STATE cho UART communication
typedef enum {
  COMMUNICATE_ERROR = 0,
  WAITING,
  TIME_OUT_STATE,
  START_SEND_DATA,
  STOP_SEND_DATA
} UartCommunicationState_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_BUFFER_SIZE  30

#define SET   1
#define RESET 0

#define TIMEOUT      3        // số bước timeout
#define TIMER_CYCLE 10        // ms, phải khớp với chu kỳ TIM2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// UART byte tạm
uint8_t temp = 0;

// buffer UART
uint8_t  buffer[MAX_BUFFER_SIZE];
uint8_t  index_buffer = 0;
uint8_t  buffer_flag  = 0;

// ADC + chuỗi gửi
uint32_t ADC_value = 0;
char     string[50];

// tín hiệu giữa 2 FSM
uint8_t start_send_data = RESET;
uint8_t stop_send_data  = RESET;

// state của 2 FSM
CommandParserState_t     commandParserState     = IDLE;
UartCommunicationState_t uartCommunicationState = WAITING;
int timeout_counter = TIMEOUT;

// timer mềm
unsigned char timer1_flag = 0;
unsigned char timer2_flag = 0;
static int timer1_counter = 0;
static int timer2_counter = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void command_parser_fsm(void);
void uart_communication_fsm(void);

void setTimer1(int duration_ms);
void setTimer2(int duration_ms);
void timer_run(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//-------------------- TIMER MỀM --------------------
void setTimer1(int duration_ms)
{
  timer1_counter = duration_ms / TIMER_CYCLE;
  if (timer1_counter <= 0) timer1_counter = 1;
  timer1_flag = 0;
}

void setTimer2(int duration_ms)
{
  timer2_counter = duration_ms / TIMER_CYCLE;
  if (timer2_counter <= 0) timer2_counter = 1;
  timer2_flag = 0;
}

void timer_run(void)
{
  if (timer1_counter > 0) {
    timer1_counter--;
    if (timer1_counter <= 0) timer1_flag = 1;
  }

  if (timer2_counter > 0) {
    timer2_counter--;
    if (timer2_counter <= 0) timer2_flag = 1;
  }
}

//-------------------- FSM PARSER --------------------
void command_parser_fsm(void)
{
  // Lấy byte mới nhất
  unsigned char bufferCheck;
  if (index_buffer == 0)
    bufferCheck = buffer[MAX_BUFFER_SIZE - 1];
  else
    bufferCheck = buffer[index_buffer - 1];

  switch (commandParserState)
  {
  case IDLE:
    if (bufferCheck == '!') commandParserState = START_PROCESS;
    break;

  case START_PROCESS:
    if (bufferCheck == 'O') commandParserState = RECEIVE_O;
    else if (bufferCheck == 'R') commandParserState = RECEIVE_R;
    else commandParserState = IDLE;
    break;

  // --- XỬ LÝ !RST# ---
  case RECEIVE_R:
    if (bufferCheck == 'S') commandParserState = RECEIVE_S;
    else commandParserState = IDLE;
    break;

  case RECEIVE_S:
    if (bufferCheck == 'T') commandParserState = RECEIVE_T;
    else commandParserState = IDLE;
    break;

  case RECEIVE_T:
    if (bufferCheck == '#') {
      start_send_data = 1;
      stop_send_data = 0;

      ADC_value = HAL_ADC_GetValue(&hadc1);
      sprintf(string, "!ADC=%lu#\r\n", (unsigned long)ADC_value);
      HAL_UART_Transmit(&huart2, (uint8_t*)string, strlen(string), 100);

      commandParserState = IDLE;
    } else {
      commandParserState = IDLE;
    }
    break;

  // --- XỬ LÝ !OK# ---
  case RECEIVE_O:
    if (bufferCheck == 'K') commandParserState = RECEIVE_K;
    else commandParserState = IDLE;
    break;

  case RECEIVE_K:
    if (bufferCheck == '#') {
      // Đã nhận đúng !OK# -> Dừng gửi
      start_send_data = 0;
      stop_send_data = 1;

      // Phản hồi STOP
      HAL_UART_Transmit(&huart2, (uint8_t *)"STOP\r\n", 6, 100);

      commandParserState = IDLE;
    } else {
      commandParserState = IDLE;
    }
    break;

  default:
    commandParserState = IDLE;
    break;
  }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // Nháy đèn để biết có nhận data

    buffer[index_buffer++] = temp;
    if (index_buffer >= MAX_BUFFER_SIZE) index_buffer = 0;

    buffer_flag = 1; // Báo hiệu cho Main loop

    HAL_UART_Receive_IT(&huart2, &temp, 1); // Tiếp tục nhận byte sau
  }
}

//-------------------- FSM UART COMMUNICATION --------------------
void uart_communication_fsm(void)
{
  switch (uartCommunicationState)
  {
  case WAITING:
    if (start_send_data == 1) {
      uartCommunicationState = START_SEND_DATA;
      setTimer2(50);
    }
    break;

  case START_SEND_DATA:
    if (stop_send_data == 1) {
      uartCommunicationState = STOP_SEND_DATA;
    }
    else if (timer2_flag == 1) {
//      ADC_value = HAL_ADC_GetValue(&hadc1);
      sprintf(string, "!ADC=%lu#\r\n", (unsigned long)ADC_value);
      HAL_UART_Transmit(&huart2, (uint8_t *)string, strlen(string), 100);

      setTimer2(3000);
    }
    break;

  case STOP_SEND_DATA:
    stop_send_data = 0;
    uartCommunicationState = WAITING;
    break;

  default:
    uartCommunicationState = WAITING;
    break;
  }
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

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
  // bật timer base interrupt
  HAL_TIM_Base_Start_IT(&htim2);

  // bật UART RX interrupt
  HAL_UART_Receive_IT(&huart2, &temp, 1);

  // bật ADC continuous
  HAL_ADC_Start(&hadc1);

  // init timer mềm
  setTimer1(1);
  setTimer2(50);
  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
    // FSM parser – chỉ chạy khi có byte mới
    if (buffer_flag) {
      command_parser_fsm();
      buffer_flag = 0;
    }

    // FSM hệ thống – chạy liên tục
    uart_communication_fsm();
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;  // terminal PC nhớ set 9600
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
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

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2) {
    timer_run();
  }
}
/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  (void)file;
  (void)line;
}
#endif /* USE_FULL_ASSERT */
