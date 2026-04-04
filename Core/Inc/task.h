// 等待完善...

#ifndef __TASK_H
#define __TASK_H

#include "main.h"
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
 * @brief 等待视觉应答
 */
__STATIC_INLINE void WaitForAck(void)
{
    while (g_cmd[0] == 'A' && g_cmd[1] == 'C' && g_cmd[2] == 'K')
    {
    }
    TIM6_Stop(); // 关闭定时器6
}

#endif
