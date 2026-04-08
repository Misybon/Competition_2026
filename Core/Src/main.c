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
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "color.h"
#include "ir.h"
#include "motor.h"
#include "pid.h"
#include "task.h"

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
extern DMA_HandleTypeDef hdma_usart3_rx;

volatile bool g_motor_startflag = 0; // 电机标志位
volatile bool g_color_status = 0; // 颜色传感器标志位

volatile bool g_status_errorflag = 0; // 状态机错误标志位
volatile bool g_vision_errorflag = 0; // 视觉错误标志位

volatile bool g_start_area_flag = 0; // 是否处于开始区
volatile bool g_throw_area_flag = 0; // 是否处于投掷区
volatile bool g_return_flag = 0; // 是否处于返回过程

volatile uint8_t g_corner_count = 0; // 转弯计数

uint8_t g_rx_data[20] = { 0 }; // 串口接收缓冲区
uint8_t g_cmd[20] = { 0 }; // 解析出来的命令

volatile TRACK_STATUS g_status = STBY; // 等待启动
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
    LL_mDelay(10); // 等待扩展板上电
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
    PID_Init();
    Color_Init();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        switch (g_status)
        {
        case STBY:
            if (!LL_GPIO_IsInputPinSet(Start_GPIO_Port, Start_Pin)) // 检测启动按钮按下
            {
                LL_mDelay(20); // 消抖
                while (LL_GPIO_IsInputPinSet(Start_GPIO_Port, Start_Pin)) // 阻塞等待释放按钮
                {
                }
                g_status = TRACK; // 进入循迹状态
            }
            break;
        case TRACK:
            if (!g_motor_startflag) // 如果电机没启动
            {
                Track_Start(); // 开始循迹
                if (!g_return_flag)
                {
                    TIM7_Start(); // 开启定时器7，用于丢线判断和PID控制
                }
            }
            break;
        case CORNER:
            Track_Break();
            if (!g_return_flag) // 如果不处于返回状态
            {
                Motor_Rot_Angle(90); // 逆时针旋转90°
            }
            else
            {
                Motor_Rot_Angle(-90); // 顺时针旋转90°
            }
            g_corner_count++;
            Track_Restart();
            g_status = TRACK; // 返回循迹状态
            break;
        case THROW_PREPARE:
            if (g_throw_area_flag) // 如果到达了投掷区
            {
                Track_Break(); // 制动
                TIM6_Stop(); // 关闭定时器6
                Color_Stop(); // 关闭颜色传感器
                g_corner_count = 0; // 重置转弯计数
                g_status = THROW_WAIT;
            }
            else if (!g_color_status) // 如果没有开启颜色传感器
            {
                Color_Start();
                TIM6_Start(); // 开启定时器6，用于定时扫描颜色
            }
            break;
        case THROW_WAIT:
            HAL_UARTEx_ReceiveToIdle_DMA(&huart3, g_rx_data, 20); // 准备接收应答
            __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
            SendReady(); // 发送准备信号
            WaitForAck(); // 等待应答
            // FindBasket();
            Throw();
            g_return_flag = 1; // 返回
            Motor_Rot_Angle(180); // 转身离开
            Track_Restart(); // 重启循迹
            while (!IsLineLost()) // 等待回到线上
            {
            }
            g_status = TRACK; // 返回循迹状态
            break;
        case STOP_PREPARE:
            if (g_start_area_flag) // 如果回到了开始区
            {
                Track_Break(); // 刹车
                TIM6_Stop(); // 关闭定时器6
                TIM7_Stop(); // 关闭定时器7
                Track_Stop(); // 关闭PWM输出
                Color_Stop(); // 关闭颜色传感器
                g_corner_count = 0; // 重置转弯计数
                g_status = STBY; // 恢复等待状态
            }
            else if (!g_color_status) // 如果没有开启颜色传感器
            {
                Color_Start(); // 唤醒颜色传感器
                TIM6_Start(); // 开启定时器6，用于定时扫描颜色
            }
            break;
        default:
            g_status_errorflag = 1;
            Error_Handler();
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
            g_status = STBY;
            break;
        }
        else if (g_vision_errorflag) // 视觉错误
        {
            // 恢复起始状态
            Track_Break();
            Track_Stop();
            g_status = STBY;
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
