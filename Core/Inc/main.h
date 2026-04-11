/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "config.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum
{
    STBY = 0,
    TRACK = 1,
    CORNER = 2,
    THROW_PREPARE = 3,
    THROW_WAIT = 4,
    STOP_PREPARE = 5,
} TRACK_STATUS;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern volatile bool g_color_status;
extern volatile bool g_return_flag;
extern volatile uint8_t g_corner_count;
extern volatile TRACK_STATUS g_status;
extern volatile bool g_start_area_flag;
extern volatile bool g_throw_area_flag;
extern volatile bool g_vision_errorflag;
extern uint8_t g_rx_data[BUF_SIZE];
volatile extern uint8_t g_cmd[BUF_SIZE];
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define COLOR_LED_Pin LL_GPIO_PIN_13
#define COLOR_LED_GPIO_Port GPIOC
#define Start_Pin LL_GPIO_PIN_2
#define Start_GPIO_Port GPIOC
#define Motor_PWM_Pin LL_GPIO_PIN_0
#define Motor_PWM_GPIO_Port GPIOA
#define Motor_PWMA1_Pin LL_GPIO_PIN_1
#define Motor_PWMA1_GPIO_Port GPIOA
#define Motor_PWMA2_Pin LL_GPIO_PIN_2
#define Motor_PWMA2_GPIO_Port GPIOA
#define Motor_PWMA3_Pin LL_GPIO_PIN_3
#define Motor_PWMA3_GPIO_Port GPIOA
#define Motor1_Con1_Pin LL_GPIO_PIN_4
#define Motor1_Con1_GPIO_Port GPIOA
#define Motor1_Con2_Pin LL_GPIO_PIN_5
#define Motor1_Con2_GPIO_Port GPIOA
#define Motor_Encoder_3_Pin LL_GPIO_PIN_6
#define Motor_Encoder_3_GPIO_Port GPIOA
#define Motor_Encoder_3A7_Pin LL_GPIO_PIN_7
#define Motor_Encoder_3A7_GPIO_Port GPIOA
#define Motor2_Con1_Pin LL_GPIO_PIN_4
#define Motor2_Con1_GPIO_Port GPIOC
#define Motor2_Con2_Pin LL_GPIO_PIN_5
#define Motor2_Con2_GPIO_Port GPIOC
#define IR1_Pin LL_GPIO_PIN_0
#define IR1_GPIO_Port GPIOB
#define IR2_Pin LL_GPIO_PIN_1
#define IR2_GPIO_Port GPIOB
#define IR3_Pin LL_GPIO_PIN_2
#define IR3_GPIO_Port GPIOB
#define IR4_Pin LL_GPIO_PIN_10
#define IR4_GPIO_Port GPIOB
#define IR5_Pin LL_GPIO_PIN_11
#define IR5_GPIO_Port GPIOB
#define Motor3_Con1_Pin LL_GPIO_PIN_12
#define Motor3_Con1_GPIO_Port GPIOB
#define Motor3_Con2_Pin LL_GPIO_PIN_13
#define Motor3_Con2_GPIO_Port GPIOB
#define Motor4_Con1_Pin LL_GPIO_PIN_14
#define Motor4_Con1_GPIO_Port GPIOB
#define Motor4_Con2_Pin LL_GPIO_PIN_15
#define Motor4_Con2_GPIO_Port GPIOB
#define Motor_PWM_Alternative_Pin LL_GPIO_PIN_6
#define Motor_PWM_Alternative_GPIO_Port GPIOC
#define Motor_PWM_AlternativeC7_Pin LL_GPIO_PIN_7
#define Motor_PWM_AlternativeC7_GPIO_Port GPIOC
#define Motor_PWM_AlternativeC8_Pin LL_GPIO_PIN_8
#define Motor_PWM_AlternativeC8_GPIO_Port GPIOC
#define Motor_PWM_AlternativeC9_Pin LL_GPIO_PIN_9
#define Motor_PWM_AlternativeC9_GPIO_Port GPIOC
#define Motor_Encoder_1_Pin LL_GPIO_PIN_8
#define Motor_Encoder_1_GPIO_Port GPIOA
#define Motor_Encoder_1A9_Pin LL_GPIO_PIN_9
#define Motor_Encoder_1A9_GPIO_Port GPIOA
#define Motor_Encoder_2_Pin LL_GPIO_PIN_15
#define Motor_Encoder_2_GPIO_Port GPIOA
#define Motor_Encoder_2B3_Pin LL_GPIO_PIN_3
#define Motor_Encoder_2B3_GPIO_Port GPIOB
#define Motor_Encoder_4_Pin LL_GPIO_PIN_6
#define Motor_Encoder_4_GPIO_Port GPIOB
#define Motor_Encoder_4B7_Pin LL_GPIO_PIN_7
#define Motor_Encoder_4B7_GPIO_Port GPIOB
#define Color_Sensor_Pin LL_GPIO_PIN_8
#define Color_Sensor_GPIO_Port GPIOB
#define Color_SensorB9_Pin LL_GPIO_PIN_9
#define Color_SensorB9_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
