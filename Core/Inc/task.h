// 等待完善...

#pragma once

#include "usart.h"

extern const uint8_t TASK_READY[5];
extern const uint8_t TASK_RETURN[5];
extern const uint8_t TASK_ACK[5];

void WaitForAck(void);
void FindBasket(void);
void Throw(void);
void SendReady(void);

/**
 * @brief 向视觉发送应答信号
 */
__STATIC_INLINE void SendAck(void)
{
    HAL_UART_Transmit(&huart3, TASK_ACK, sizeof(TASK_ACK) / sizeof(uint8_t), 100);
}

/**
 * @brief 向视觉发送返回信号
 */
__STATIC_INLINE void SendReturn(void)
{
    HAL_UART_Transmit(&huart3, TASK_RETURN, sizeof(TASK_RETURN) / sizeof(uint8_t), 100);
}
