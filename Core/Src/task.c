// 等待完善...

#include "task.h"
#include "config.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "track.h"

static volatile bool s_vision_errorflag = 0; // 视觉错误标志位

// 加校验和位
const uint8_t TASK_READY[5] = { 0xAA, 0x55, 0x01, 0x01, 0x02 };
const uint8_t TASK_RETURN[5] = { 0xAA, 0x55, 0x01, 0x02, 0x03 };
const uint8_t TASK_ACK[5] = { 0xAA, 0x55, 0x01, 'A', 0x42 };

/**
 * @brief 向视觉发送准备信号并等待应答
 */
void SendReady(void)
{
    uint32_t tick_start = HAL_GetTick();
    uint32_t retry = 0; // 重试次数

    while (!(g_cmd[0] == 'A' && g_cmd[1] == 'C' && g_cmd[2] == 'K')) // 等待收到应答命令
    {
        if (HAL_GetTick() - tick_start >= 300) // 300ms超时
        {
            tick_start = HAL_GetTick();
            if (retry >= 10) // 重试次数过多，认定视觉掉线
            {
                s_vision_errorflag = 1; // 设定标志位
                return;
            }
            HAL_UART_Transmit(&huart3, TASK_READY, sizeof(TASK_READY) / sizeof(uint8_t), 100); // 重发准备信号
            retry++; // 重试计数+1
        }
    }

    // 清除应答命令
    g_cmd[0] = '\0';
    g_cmd[1] = '\0';
    g_cmd[2] = '\0';
}

/**
 * @brief 寻找篮筐函数，超时10s
 */
void FindBasket(void)
{
    uint32_t tick_start = HAL_GetTick();
    uint32_t timeout_cnt = 0; // 超时计数

    while (1)
    {
        if (s_vision_errorflag) // 如果视觉掉线
        {
            break; // 放弃寻找篮筐
        }
        else if (g_cmd[5]) // 没找到篮筐
        {
            g_track_speed.vz = MAX_VZ; // 最大速度扫描篮筐
        }
        else if (g_cmd[4]) // 对准篮筐
        {
            Track_Break(); // 制动
            break; // 返回
        }
        else // 找到篮筐但没对准
        {
            uint32_t offset = (uint32_t)0x0000 | (uint32_t)g_cmd[0] | (uint32_t)g_cmd[1] << 8; // 拼接偏移值
            g_track_speed.vz = OFFSET_KP * offset; // 乘上比例赋予角速度
        }

        if (HAL_GetTick() - tick_start >= 1000) // 单次超时1000ms
        {
            tick_start = HAL_GetTick();
            timeout_cnt++;
            if (timeout_cnt >= 10) // 10s后仍没找到篮筐就放弃
            {
                Track_Break(); // 直接制动
                break; // 返回
            }
        }
    }

    HAL_UART_AbortReceive_IT(&huart3); // 关闭串口接收
}

/**
 * @brief 投掷弹丸并向视觉发送返回指令
 * 
 */
void Throw(void)
{
    // 投掷代码
    LL_GPIO_SetOutputPin(Motor5_GPIO_Port, Motor5_Pin);
    LL_mDelay(1000); // 根据经验修改
    LL_GPIO_ResetOutputPin(Motor5_GPIO_Port, Motor5_Pin);

    if (!s_vision_errorflag) // 如果视觉没有掉线
    {
        SendReturn(); // 发送返回指令
    }
}
