// 等待完善...

#include "task.h"
#include "config.h"
#include "main.h"
#include "state_handler.h"
#include "track.h"

static volatile bool s_vision_errorflag = 0; // 视觉错误标志位

const uint8_t TASK_READY[5] = { 0xAA, 0x55, 0x01, 0x01, 0x02 }; // 准备完毕数据包
const uint8_t TASK_RETURN[5] = { 0xAA, 0x55, 0x01, 0x02, 0x03 }; // 返回数据包
const uint8_t TASK_ACK[5] = { 0xAA, 0x55, 0x01, 'A', 0x42 }; // 应答数据包

/**
 * @brief 向视觉发送准备信号并等待应答
 */
void SendReady(void)
{
    uint32_t tick_start = HAL_GetTick();
    uint32_t retry = 0; // 重试次数

    HAL_UART_Transmit(&huart3, TASK_READY, sizeof(TASK_READY), 100);

    while (1) // 等待收到应答命令
    {
        if (g_usart_flag) // 解析出新命令
        {
            g_usart_flag = 0; // 清除标志位

            if (g_cmd[0] == 'A') // 收到应答命令
            {
                break;
            }
            else // 否则清除命令
            {
                for (uint32_t i = 0; i < BUF_SIZE; i++)
                {
                    g_cmd[i] = '\0';
                }
            }
        }

        if (HAL_GetTick() - tick_start >= 300) // 300ms超时
        {
            tick_start = HAL_GetTick();
            if (retry >= 10) // 重试次数过多，认定视觉掉线
            {
                s_vision_errorflag = 1; // 设定标志位

                // 清除命令
                for (uint32_t i = 0; i < BUF_SIZE; i++)
                {
                    g_cmd[i] = '\0';
                }

                HAL_UART_AbortReceive_IT(&huart3); // 关闭串口接收

                return;
            }
            HAL_UART_Transmit(&huart3, TASK_READY, sizeof(TASK_READY) / sizeof(uint8_t), 50); // 重发准备信号
            retry++; // 重试计数+1
        }
    }

    for (uint32_t i = 0; i < BUF_SIZE; i++) // 清除命令
    {
        g_cmd[i] = '\0';
    }

    s_vision_errorflag = 0; // 视觉在线
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

        if (g_usart_flag) // 解析出新命令
        {
            g_usart_flag = 0; // 清除标志位

            if (g_cmd[5]) // 没找到篮筐
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
                int32_t offset = (int16_t)((uint16_t)g_cmd[0] | ((uint16_t)g_cmd[1] << 8)); // 拼接位移值
                g_track_speed.vz = OFFSET_KP * offset; // 乘上比例赋予角速度
            }

            HAL_UART_Transmit_DMA(&huart3, TASK_ACK, sizeof(TASK_ACK));
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
        HAL_UART_AbortReceive_IT(&huart3); // 关闭串口接收
    }
}
