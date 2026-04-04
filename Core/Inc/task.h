// 等待完善...

#ifndef __TASK_H
#define __TASK_H

#include "main.h"
#include "stm32f1xx_hal.h"
#include "usart.h"

extern const uint8_t TASK_READY[9];
extern const uint8_t TASK_RETURN[10];

void Throw(void);

/**
 * @brief 向视觉发送准备信号
 */
__STATIC_INLINE void SendReady(void)
{
    HAL_UART_Transmit(&huart3, TASK_READY, sizeof(TASK_READY) / sizeof(uint8_t), 100);
}

/**
 * @brief 向视觉发送返回信号
 */
__STATIC_INLINE void SendReturn(void)
{
    HAL_UART_Transmit(&huart3, TASK_RETURN, sizeof(TASK_RETURN) / sizeof(uint8_t), 100);
}

/**
 * @brief 等待视觉应答，1000毫秒超时，重试三次
 */
__STATIC_INLINE void WaitForAck(void)
{
    uint32_t tick_start = HAL_GetTick();
    uint32_t retry = 0;
    while (!(g_cmd[0] == 'A' && g_cmd[1] == 'C' && g_cmd[2] == 'K'))
    {
        if (HAL_GetTick() - tick_start >= 1000)
        {
            tick_start = HAL_GetTick();
            if (retry >= 3)
            {
                g_vision_errorflag = 1;
                Error_Handler();
                return;
            }
            SendReady();
            retry++;
        }
    }
}

#endif
