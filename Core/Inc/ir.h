#ifndef __IR_H
#define __IR_H

#include "main.h"

/**
 * @brief 判断是否丢线
 * 
 * @return 丢线则返回1
 */
__STATIC_INLINE uint32_t IsLineLost(void)
{
    return (LL_GPIO_IsInputPinSet(IR1_GPIO_Port, IR1_Pin) || LL_GPIO_IsInputPinSet(IR2_GPIO_Port, IR2_Pin)
            || LL_GPIO_IsInputPinSet(IR3_GPIO_Port, IR3_Pin) || LL_GPIO_IsInputPinSet(IR4_GPIO_Port, IR4_Pin) || LL_GPIO_IsInputPinSet(IR5_GPIO_Port, IR5_Pin));
}

#endif
