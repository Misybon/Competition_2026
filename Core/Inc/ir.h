#ifndef __IR_H
#define __IR_H

#include "main.h"

struct IR_Val
{
    bool _1;
    bool _2;
    bool _3;
    bool _4;
    bool _5;
};

extern int32_t g_ir_out;

extern struct IR_Val g_ir_val;

/**
 * @brief 获取红外传感器值，黑为0，否则为1，存入g_ir_val
 */
__STATIC_INLINE void IR_GetVal(void)
{
    g_ir_val._1 = LL_GPIO_IsInputPinSet(IR1_GPIO_Port, IR1_Pin);
    g_ir_val._2 = LL_GPIO_IsInputPinSet(IR2_GPIO_Port, IR2_Pin);
    g_ir_val._3 = LL_GPIO_IsInputPinSet(IR3_GPIO_Port, IR3_Pin);
    g_ir_val._4 = LL_GPIO_IsInputPinSet(IR4_GPIO_Port, IR4_Pin);
    g_ir_val._5 = LL_GPIO_IsInputPinSet(IR5_GPIO_Port, IR5_Pin);
}

/**
 * @brief 判断是否丢线
 * 
 * @return 丢线则返回1
 */
__STATIC_INLINE uint32_t IsLineLost(void)
{
    return (g_ir_val._1 || g_ir_val._2 || g_ir_val._3 || g_ir_val._4 || g_ir_val._5);
}

#endif
