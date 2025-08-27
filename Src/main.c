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
#include "usart.h"
#include "gpio.h"

#include "referee_protocol.h"
#include "referee_frame_process.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "stdint.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_KEYS 3
#define DELAY_MS 25
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
bool key_pressed[NUM_KEYS] = {false};
bool key_flag[NUM_KEYS] = {true}; 

custom_client_data_t custom_client_data = {0};
uint8_t client_data[8] = {0};

typedef struct {
    uint16_t x_pos;
    uint16_t y_pos;
} Position;

const Position key_positions[NUM_KEYS] = {
    {1160, 560},  //50弹
    {1210, 560},  //100弹
    {1210, 560}   
};

const Position common_positions[3] = {
    {960, 670},   
    {860, 560}  //确认
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void send_mouse_sequence(uint16_t x, uint16_t y);
void send_common_sequence(void);
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
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART3_UART_Init();

    while (1)
    {
        for (int i = 0; i < NUM_KEYS; i++) {
            GPIO_PinState pin_state;
            
            switch (i) {
                case 0: pin_state = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_50_Pin); break;
                case 1: pin_state = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_100_Pin); break;
                case 2: pin_state = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_200_Pin); break;
            }
            
            key_pressed[i] = (pin_state == GPIO_PIN_RESET);
            if (pin_state == GPIO_PIN_SET) {
                key_flag[i] = false;
            }
            
            if (key_pressed[i] && !key_flag[i]) {
				   custom_client_data = (custom_client_data_t){
					.key_value = 79,
					.x_position = 0,
					.y_position = 0,
					.mouse_left = 0,
					.mouse_right = 0
				};
				memcpy(client_data, &custom_client_data, 8);
				HAL_UART_Transmit(&huart3, referee_pack_data(0x0306, client_data, 8), getRefSentDataLen(), 30);
				HAL_Delay(DELAY_MS);
    
                send_mouse_sequence(key_positions[i].x_pos, key_positions[i].y_pos);
                if(i == 2)
                send_mouse_sequence(key_positions[i].x_pos, key_positions[i].y_pos);
				
                send_common_sequence();
                
				custom_client_data = (custom_client_data_t){
					.key_value = 79,
					.x_position = 0,
					.y_position = 0,
					.mouse_left = 0,
					.mouse_right = 0
				};
				memcpy(client_data, &custom_client_data, 8);
				HAL_UART_Transmit(&huart3, referee_pack_data(0x0306, client_data, 8), getRefSentDataLen(), 30);
				HAL_Delay(DELAY_MS);
				
                send_mouse_sequence(0, 0);
				
                key_flag[i] = true;
                key_pressed[i] = false;
            }
        }
			HAL_Delay(10);
    }
}

/**
 * @brief 鼠标点击操作
 * @param x X coordinate
 * @param y Y coordinate
 */
void send_mouse_sequence(uint16_t x, uint16_t y)
{
    custom_client_data = (custom_client_data_t){
        .key_value = 0,			//网上任意键值
        .x_position = x,
        .y_position = y,
        .mouse_left = 0,
        .mouse_right = 0
    };
    memcpy(client_data, &custom_client_data, 8);
    HAL_UART_Transmit(&huart3, referee_pack_data(0x0306, client_data, 8), getRefSentDataLen(), 30);
    HAL_Delay(DELAY_MS);
    
    custom_client_data.mouse_left = 1;
    memcpy(client_data, &custom_client_data, 8);
    HAL_UART_Transmit(&huart3, referee_pack_data(0x0306, client_data, 8), getRefSentDataLen(), 30);
    HAL_Delay(DELAY_MS);
    
    custom_client_data.mouse_left = 0;
    memcpy(client_data, &custom_client_data, 8);
    HAL_UART_Transmit(&huart3, referee_pack_data(0x0306, client_data, 8), getRefSentDataLen(), 30);
    HAL_Delay(DELAY_MS);
}

/**
 * @brief Sends the common mouse sequence shared by all keys
 */
void send_common_sequence(void)
{
    send_mouse_sequence(common_positions[0].x_pos, common_positions[0].y_pos);
    
    send_mouse_sequence(common_positions[1].x_pos, common_positions[1].y_pos);
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
