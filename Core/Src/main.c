/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "gpio.h"
#include "i2c.h"
#include "stm32f1xx_ll_utils.h"
#include "tim.h"
#include "track.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pid.h"
#include "state_handler.h"

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
volatile TRACK_STATUS g_status = STBY; // 等待启动
extern volatile bool g_status_errorflag; // 状态机错误标志位
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/**
 * @brief 重置状态机
 */
void Status_Reset(void)
{
    g_status = STBY; // 恢复等待状态
    g_return_flag = 0; // 清除返回状态标志位
    g_corner_count = 0; // 清零转弯计数
    g_line_reached = 0; // 清除到达线上标志位
    g_color_status = 0; // 清除颜色传感器状态标志位
    g_motor_startflag = 0; // 清除电机启动标志位
    PID_Init(); // 再次初始化PID
}
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
    MX_DMA_Init();
    MX_I2C1_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_TIM5_Init();
    MX_TIM8_Init();
    MX_USART3_UART_Init();
    MX_TIM6_Init();
    MX_TIM7_Init();
    /* USER CODE BEGIN 2 */
    PID_Init(); // 初始化PID参数

    Color_Init(); // 初始化颜色传感器，是否加入设备识别错误处理有待考量...

    // Motor1_Start();
    // Motor2_Start();
    // Motor3_Start();
    // Motor4_Start();
    // Color_Start();
    LL_mDelay(2000);

    TIM7_Start();
    Track_Start();
    LL_mDelay(2000);
    Track_Break();
    // LL_mDelay(10);
    g_track_speed.vz = MAX_VZ;
    LL_mDelay(2000);
    Track_Break();
    Track_Stop();

    // TIM6_Start();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        // 注意：可能因为PID调控导致转弯处无法完全丢线！

        // g_motor_tgtspeed._1 = 999;
        // g_motor_tgtspeed._2 = 999;
        // g_motor_tgtspeed._3 = 999;
        // g_motor_tgtspeed._4 = 999;
        // LL_mDelay(2000);
        // g_motor_tgtspeed._1 = 800;
        // g_motor_tgtspeed._2 = 800;
        // g_motor_tgtspeed._3 = 800;
        // g_motor_tgtspeed._4 = 800;
        // LL_mDelay(2000);
        // g_motor_tgtspeed._1 = 600;
        // g_motor_tgtspeed._2 = 600;
        // g_motor_tgtspeed._3 = 600;
        // g_motor_tgtspeed._4 = 600;
        // LL_mDelay(2000);
        // g_motor_tgtspeed._1 = 400;
        // g_motor_tgtspeed._2 = 400;
        // g_motor_tgtspeed._3 = 400;
        // g_motor_tgtspeed._4 = 400;
        // LL_mDelay(2000);
        // g_motor_tgtspeed._1 = 200;
        // g_motor_tgtspeed._2 = 200;
        // g_motor_tgtspeed._3 = 200;
        // g_motor_tgtspeed._4 = 200;
        // LL_mDelay(2000);
        // g_motor_tgtspeed._1 = 0;
        // g_motor_tgtspeed._2 = 0;
        // g_motor_tgtspeed._3 = 0;
        // g_motor_tgtspeed._4 = 0;
        // LL_mDelay(2000);
        // g_motor_tgtspeed._1 = -200;
        // g_motor_tgtspeed._2 = -200;
        // g_motor_tgtspeed._3 = -200;
        // g_motor_tgtspeed._4 = -200;
        // LL_mDelay(2000);
        // g_motor_tgtspeed._1 = -400;
        // g_motor_tgtspeed._2 = -400;
        // g_motor_tgtspeed._3 = -400;
        // g_motor_tgtspeed._4 = -400;
        // LL_mDelay(2000);
        // g_motor_tgtspeed._1 = -600;
        // g_motor_tgtspeed._2 = -600;
        // g_motor_tgtspeed._3 = -600;
        // g_motor_tgtspeed._4 = -600;
        // LL_mDelay(2000);
        // g_motor_tgtspeed._1 = -800;
        // g_motor_tgtspeed._2 = -800;
        // g_motor_tgtspeed._3 = -800;
        // g_motor_tgtspeed._4 = -800;
        // LL_mDelay(2000);
        // g_motor_tgtspeed._1 = -999;
        // g_motor_tgtspeed._2 = -999;
        // g_motor_tgtspeed._3 = -999;
        // g_motor_tgtspeed._4 = -999;
        // LL_mDelay(2000);
        // g_motor_tgtspeed._1 = 0;
        // g_motor_tgtspeed._2 = 0;
        // g_motor_tgtspeed._3 = 0;
        // g_motor_tgtspeed._4 = 0;
        // LL_mDelay(2000);

        switch (g_status)
        {
        case STBY: // 等待状态
            // STBY_Handler();
            break;
        case TRACK: // 循迹状态
            TRACK_Handler();
            break;
        case CORNER: // 转角状态
            CORNER_Handler();
            break;
        case THROW_PREPARE: // 准备进入投掷区
            THROW_PREPARE_Handler();
            break;
        case THROW_WAIT: // 准备投掷
            THROW_WAIT_Handler();
            break;
        case STOP_PREPARE: // 准备进入开始区
            STOP_PREPARE_Handler();
            break;
        default: // 状态出错
            g_status_errorflag = 1; // 状态机出错
            Error_Handler(); // 进入错误处理，尝试恢复等待状态
            break;
        }
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
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
    {
    }
    LL_RCC_HSE_Enable();

    /* Wait till HSE is ready */
    while (LL_RCC_HSE_IsReady() != 1)
    {
    }
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLL_IsReady() != 1)
    {
    }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    /* Wait till System clock is ready */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    }
    LL_SetSystemCoreClock(72000000);

    /* Update the time base */
    if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
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
        if (g_status_errorflag) // 状态错误
        {
            // 恢复起始状态
            Track_Break();
            Track_Stop();
            Status_Reset();
            g_status_errorflag = 0;
            break;
        }
    }
    __enable_irq();
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
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
