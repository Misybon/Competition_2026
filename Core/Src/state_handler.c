#include "state_handler.h"
#include "color.h"
#include "ir.h"
#include "main.h"
#include "task.h"
#include "track.h"

volatile bool g_motor_startflag = 0; // 电机标志位
volatile bool g_color_status = 0; // 颜色传感器标志位

volatile bool g_status_errorflag = 0; // 状态机错误标志位

volatile bool g_line_reached = 0; // 是否已经到达线上
volatile bool g_return_flag = 0; // 是否处于返回过程

volatile uint8_t g_corner_count = 0; // 转弯计数

uint8_t g_rx_data[BUF_SIZE] = { 0 }; // 串口接收缓冲区
volatile uint8_t g_cmd[BUF_SIZE] = { 0 }; // 解析出来的命令

volatile bool g_usart_flag = 0; // 是否解析出一帧命令

/**
 * @brief 等待状态处理函数
 */
void STBY_Handler(void)
{
    if (LL_GPIO_IsInputPinSet(Start_GPIO_Port, Start_Pin)) // 检测启动按钮按下
    {
        uint32_t tick_start = HAL_GetTick();

        LL_mDelay(20); // 消抖
        while (LL_GPIO_IsInputPinSet(Start_GPIO_Port, Start_Pin)) // 阻塞等待释放按钮
        {
            if (HAL_GetTick() - tick_start >= 2000) // 超时2000ms
            {
                break; // 直接启动
            }
        }
        g_status = TRACK; // 进入循迹状态
    }
}

/**
 * @brief 循迹状态处理函数
 */
void TRACK_Handler(void)
{
    if (!g_motor_startflag) // 如果电机没启动
    {
        if (!g_return_flag) // 如果不在返回状态
        {
            TIM7_Start(); // 开启定时器7，用于丢线判断和PID控制
        }
        Track_Start(); // 开始循迹
        while (!IsLineLost()) // 先进入线上
        {
        }
        g_line_reached = 1; // 设置标志位
    }
}

/**
 * @brief 转角状态处理函数
 */
void CORNER_Handler(void)
{
    Track_Break(); // 制动
    if (!g_return_flag) // 如果不处于返回状态
    {
        Track_Rot_Angle(-90); // 顺时针旋转90°
    }
    else // 处于返回状态
    {
        Track_Rot_Angle(90); // 逆时针旋转90°
    }
    g_corner_count++; // 转弯计数+1
    Track_Restart(); // 重启循迹
    g_status = TRACK; // 返回循迹状态
}

/**
 * @brief 投掷准备状态处理函数
 */
void THROW_PREPARE_Handler(void)
{
    if (IsThorwAreaReached()) // 如果到达了投掷区
    {
        Track_Break(); // 制动
        TIM6_Stop(); // 关闭定时器6
        Color_Stop(); // 关闭颜色传感器
        g_corner_count = 0; // 重置转弯计数
        g_status = THROW_WAIT; // 进入准备投掷状态
    }
    else if (!g_color_status) // 如果没有开启颜色传感器
    {
        Color_Start(); // 唤醒颜色传感器
        TIM6_Start(); // 开启定时器6，用于定时扫描颜色
    }
}

/**
 * @brief 投掷等待状态处理函数
 */
void THROW_WAIT_Handler(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, g_rx_data, sizeof(g_rx_data) / sizeof(uint8_t)); // 准备接收应答
    __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT); // 关闭半传输中断
    SendReady(); // 发送准备信号
    FindBasket(); // 寻找篮筐
    Throw(); // 投掷弹丸
    g_return_flag = 1; // 设置返回标志位
    Track_Rot_Angle(180); // 转身离开
    Track_Restart(); // 重启循迹
    g_status = TRACK; // 返回循迹状态
}

/**
 * @brief 停止等待状态处理函数
 */
void STOP_PREPARE_Handler(void)
{
    if (IsStartAreaReached()) // 如果回到了开始区
    {
        Track_Break(); // 制动
        TIM6_Stop(); // 关闭定时器6
        TIM7_Stop(); // 关闭定时器7
        Track_Stop(); // 关闭PWM输出
        Color_Stop(); // 关闭颜色传感器
        LL_mDelay(1000); // 等待制动完成
        Status_Reset(); // 重置
    }
    else if (!g_color_status) // 如果没有开启颜色传感器
    {
        Color_Start(); // 唤醒颜色传感器
        TIM6_Start(); // 开启定时器6，用于定时扫描颜色
    }
}
