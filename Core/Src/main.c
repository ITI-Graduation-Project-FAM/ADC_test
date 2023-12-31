/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
ADC_HandleTypeDef hadc1;

CAN_HandleTypeDef hcan;

/* USER CODE BEGIN PV */

CAN_TxHeaderTypeDef MTxHeader;
CAN_TxHeaderTypeDef TxHeader;
CAN_TxHeaderTypeDef TxHeader2;
CAN_TxHeaderTypeDef TxHeader3;
CAN_TxHeaderTypeDef TxHeader4;
CAN_TxHeaderTypeDef TxHeader5;
CAN_RxHeaderTypeDef RxHeader;

CAN_TxHeaderTypeDef tempTxHeader;

uint8_t TxData[1]={1};
uint8_t TxData2[8]="Abdalla";
uint8_t TxData3[8]="Ahmed  ";
uint8_t TxData4[8]="Fatima";
uint8_t TxData5[8]="Mohamed";
uint8_t RxData[8];
uint32_t Mailbox=CAN_TX_MAILBOX0;
uint8_t resend=0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_CAN_Init(void);
/* USER CODE BEGIN PFP */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
		uint8_t tempid[30]={0};
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
		tempTxHeader.DLC = RxHeader.DLC;
		tempTxHeader.ExtId =RxHeader.ExtId;
		tempTxHeader.IDE = RxHeader.IDE;
		tempTxHeader.RTR =RxHeader.RTR;
		tempTxHeader.TransmitGlobalTime =TxHeader.TransmitGlobalTime;
		tempTxHeader.StdId =RxHeader.StdId;
		 HAL_CAN_AddTxMessage(hcan, &tempTxHeader, RxData, &Mailbox);

}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void ADC_Select_CH0 ()
{
	  ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_0;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

}

void ADC_Select_CH1 ()
{
	  ADC_ChannelConfTypeDef sConfig = {0};
	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}
void ADC_Select_CH2 ()
{
	  ADC_ChannelConfTypeDef sConfig = {0};
	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL_2;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

}

uint16_t ADC_Val[3];

//uint16_t readValue;


#define TwoPowerResolution	4096   //12 bit res
//---------------------Temperature sensor----------------------------//
uint8_t temp_val;
#define Temp_Vref 3000
//---------------------voltage sensor----------------------------//
#define Volt_Vref 3.4  //volt
float volt_val;
float volt_divider_val=5;
float volt=0.0;
uint8_t check=0;
uint8_t single_SOC;
uint8_t triple_SOC;
//  R1=8k;
//  R2=2k;


//---------------------current sensor----------------------------//
//website
//https://www.engineersgarage.com/acs712-current-sensor-with-arduino/
//http://www.energiazero.org/arduino_sensori/acs712%2030a%20range%20current%20sensor.pdf
// voltage divider : Vcc = Vout * 2000/3000  R1=1k R2=2k
float current_val;
float current_adc_volt;
float current_adc_raw;

#define Current_Vref 5000  //current
#define current_offset	2500 //Vcc/2
#define current_Sensitivity	185
//-------------------HW protection links---------------------//
//https://www.circuits-diy.com/short-circuit-protection-circuit/

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
  MX_ADC1_Init();
  MX_CAN_Init();
  /* USER CODE BEGIN 2 */
  HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING);
  HAL_CAN_Start(&hcan);
  TxHeader.DLC = 1;
   TxHeader.ExtId = 0;
   TxHeader.IDE = CAN_ID_STD;
   TxHeader.RTR = CAN_RTR_DATA;
   TxHeader.TransmitGlobalTime = DISABLE;
   TxHeader.StdId = 0x1;

   TxHeader2 =TxHeader;
   TxHeader3 =TxHeader;
   TxHeader4 =TxHeader;
   TxHeader5 =TxHeader;

   TxHeader2.StdId=0x2;
   TxHeader2.DLC=8;
   TxHeader3.StdId=0x3;
   TxHeader3.DLC=8;
   TxHeader4.StdId=0x4;
   TxHeader4.DLC=8;
   TxHeader5.StdId=0x5;
   TxHeader5.DLC=8;
   HAL_StatusTypeDef CANSvar=HAL_ERROR;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	    //------------------------------temperature-----------------------------------//
	    ADC_Select_CH0();
	    HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,1000);
		ADC_Val[0] = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop(&hadc1);
		temp_val = ((ADC_Val[0]* Temp_Vref )/TwoPowerResolution)/10;	/* LM35 gives output of 10mv/°C */
	    //------------------------------volt-----------------------------------//
		ADC_Select_CH1();
	    HAL_ADC_Start(&hadc1);
	    HAL_ADC_PollForConversion(&hadc1,1000);
	    ADC_Val[1] = HAL_ADC_GetValue(&hadc1);
	    HAL_ADC_Stop(&hadc1);
	    volt = (((float)ADC_Val[1] * Volt_Vref)*volt_divider_val )/TwoPowerResolution;	/* voltage */
	    single_SOC=(volt-3)*100;
	    triple_SOC=((volt-9)*100)/3;
//		volt=volt_val*5;  //(R2/(R1+R2))
	    //------------------------------current-----------------------------------//
	    ADC_Select_CH2();
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,1000);
		ADC_Val[2] = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop(&hadc1);
		current_adc_raw = HAL_ADC_GetValue(&hadc1);
		current_adc_volt = (current_adc_raw*Current_Vref)/TwoPowerResolution;
 		current_val =(current_adc_volt - current_offset)/current_Sensitivity ;
 		//pin3=set >connect to system
 		//pin3=reset >connect to float
 		//pin4=set >connect to float
 		//pin4=reset >connect to charger
 		if((uint8_t)volt<=9)
 		{
 			//disconnect system (under voltage) and connect charger
 			check=1;
 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
 		}
 		else if((uint8_t)volt>12)
 		{
 			//disconnect charger (over voltage) max PWM
 			check=2;
 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
 		}
 		else if(temp_val>50)
 		{
 			//disconnect all
 			check=3;
 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
 		}
 		else if((uint8_t)current_val>1)
 		{
 			//disconnect all
 			check=4;
 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
 		}
 		else if((uint8_t)current_val>0.8)
 		{
 			//warning
 			check=5;

 		}
 		else if(50>temp_val && temp_val>45)
 		{
 			//PWM 40%
 			check=6;

 		}
 		else if(45>temp_val && temp_val>40)
 		{
 			//PWM 60%
 			check=7;

 		}
 		else if(40>triple_SOC && triple_SOC>10)
 		{
 			//PWM 40% + warning
 			check=8;

 		}
 		else if(40>temp_val && temp_val>35)
 		{
 			//PWM 80% + warning
 			check=9;

 		}
 		else
 		{
 			check=10;
 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_SET);
 		}


// 		HAL_UART_Transmit(&huart1, TxData, sizeof(TxData), 1000);
// 		HAL_UART_Transmit(&huart1,(const uint8_t *)"\n", 2, 1000);
// 		HAL_UART_Transmit(&huart1, ADC_Val[1], sizeof(ADC_Val), 1000);
// 		HAL_UART_Transmit(&huart1, "\n", 1, 1000);
// 		HAL_UART_Transmit(&huart1, ADC_Val[2], sizeof(ADC_Val), 1000);
// 		HAL_UART_Transmit(&huart1, "\n", 1, 1000);

 		TxData[0]=(char)temp_val;
 		MTxHeader.DLC=1;
 		MTxHeader.StdId=0x10;
 		CANSvar=HAL_CAN_AddTxMessage(&hcan, &MTxHeader, TxData, &Mailbox);
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		  HAL_Delay(500);
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		  HAL_Delay(500);

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
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

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

//  /** Configure Regular Channel
//  */
//  sConfig.Channel = ADC_CHANNEL_0;
//  sConfig.Rank = ADC_REGULAR_RANK_1;
//  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  /** Configure Regular Channel
//  */
//  sConfig.Channel = ADC_CHANNEL_1;
//  sConfig.Rank = ADC_REGULAR_RANK_2;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  /** Configure Regular Channel
//  */
//  sConfig.Channel = ADC_CHANNEL_2;
//  sConfig.Rank = ADC_REGULAR_RANK_3;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
  /* USER CODE BEGIN ADC1_Init 2 */
//
  /* USER CODE END ADC1_Init 2 */


}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 18;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_3TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
  CAN_FilterTypeDef CanFilterConfig;
    CanFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
    CanFilterConfig.FilterBank = 0;
    CanFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    CanFilterConfig.FilterIdHigh = 0;
    CanFilterConfig.FilterIdLow = 0;
    CanFilterConfig.FilterMaskIdHigh = 0;
    CanFilterConfig.FilterMaskIdLow = 0;
    CanFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    CanFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    CanFilterConfig.SlaveStartFilterBank = 2;

    HAL_CAN_ConfigFilter(&hcan, &CanFilterConfig);

  /* USER CODE END CAN_Init 2 */


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
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */

  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();


  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA3 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


/* USER CODE BEGIN MX_GPIO_Init_2 */
	__HAL_RCC_GPIOC_CLK_ENABLE();
  /*Configure GPIO pin Output Level */
   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

   /*Configure GPIO pin : PC13 */
   GPIO_InitStruct.Pin = GPIO_PIN_13;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
