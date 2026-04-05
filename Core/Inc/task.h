// 等待完善...

#ifndef __TASK_H
#define __TASK_H

#include "main.h"
#include "stm32f1xx_hal.h"
#include "usart.h"

extern const uint8_t TASK_READY[9];
extern const uint8_t TASK_RETURN[10];

void Throw(void);
void WaitForAck(void);

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

#endif
