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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TSL2591.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
// Sensor type struct
	void *sensorStruct;
//	Sensor Data
    float value1;
    int value2;
    float lux;
	uint16_t atime;
	uint16_t again;
} sensorData;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern uint8_t DEV_ModuleInit(I2C_HandleTypeDef *i2c_handle, uint8_t address);


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t sec_mark_led;
//float lux;
//int ir;
sensorData LuxSensor1;
sensorData LuxSensor2;
TSL2591_t TSL_LuxSensor1;
TSL2591_t sensor;
TSL2591_t TSL_LuxSensor2;
float lux;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  /*  -------------TSL2591 Sensor Init--------------------------------------------*/

  DEV_I2C_Init(&hi2c1, TSL2591_ADDRESS);
//  TSL2591_Init(&TSL_LuxSensor1, &hi2c1, TSL2591_ADDRESS);
  TSL2591_Init(&TSL_LuxSensor1, &hi2c1, TSL2591_ADDRESS);

  DEV_I2C_Init(&hi2c2, TSL2591_ADDRESS);
  TSL2591_Init(&TSL_LuxSensor2, &hi2c2, TSL2591_ADDRESS);
   /*  --------------Gain Adjust TSL2591-------------------------------------------*/

  TSL2591_Set_Gain(&TSL_LuxSensor1, LOW_AGAIN);
  TSL2591_Set_Gain(&TSL_LuxSensor2, LOW_AGAIN);
  /*  ---------------Atime Adjust TSL2591------------------------------------------*/

  TSL2591_Set_IntegralTime(&TSL_LuxSensor1, ATIME_500MS);
  TSL2591_Set_IntegralTime(&TSL_LuxSensor2, ATIME_500MS);



  /*  ---------------Interrupts TSL2591------------------------------------------*/


//  TSL2591_SET_InterruptThreshold(&TSL_LuxSensor1, 0x000f, 0xff00);
//  TSL2591_SET_InterruptThreshold(&TSL_LuxSensor2, 0x000f, 0xff00);
  TSL2591_SET_LuxInterrupt(&TSL_LuxSensor1, 150, 250);
  TSL2591_SET_LuxInterrupt(&TSL_LuxSensor2, 150, 250);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TEST_LED_Ir_GPIO_Port, TEST_LED_Ir_Pin, GPIO_PIN_SET);

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  /*  ---------------TSL2591 Data read------------------------------------------*/

	  LuxSensor1.value1 = TSL2591_Read_Lux(&TSL_LuxSensor1); // Lux data for Tsl Sensor 1
	  LuxSensor1.value2 = TSL2591_Read_Channel1(&TSL_LuxSensor1);
//	  LuxSensor1.value1 = TSL_LuxSensor1.data_read1;    // Lux data read for Tsl Sensor 1
//	  LuxSensor1.value2 = TSL_LuxSensor1.data_read2;   // Ir Channel data read for Tsl Sensor 1

	  LuxSensor1.value1 = TSL2591_Read_Lux(&TSL_LuxSensor2);
	  LuxSensor1.value2 = TSL2591_Read_Channel1(&TSL_LuxSensor2); // Lux data for Tsl Sensor 1
//	  LuxSensor1.value1 = TSL_LuxSensor2.data_read1;    // Lux data read for Tsl Sensor 1
//	  LuxSensor1.value2 = TSL_LuxSensor2.data_read2;   // Ir Channel data read for Tsl Sensor 1


      //	  LuxSensor1.value2 = TSL2591_Read_Channel1(sensor);
//	  TslSens.atime = TSL2591_Set_IntegralTime(ATIME_500MS);

		TSL2591_SET_LuxInterrupt(&TSL_LuxSensor1, 50,200);
		TSL2591_SET_LuxInterrupt(&TSL_LuxSensor2, 50,200);


		if(HAL_GetTick() - sec_mark_led >= 900){
			HAL_GPIO_TogglePin(TEST_LED_GPIO_Port, TEST_LED_Pin);
			sec_mark_led = HAL_GetTick();
		}

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
