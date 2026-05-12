#pragma once

#include "main.h"

/**
 * @brief 红外状态值
 */
typedef enum
{
    IR_LOST = -127, // 丢线
    IR_ERR = -126, // 错误

    IR_OK = 0, // 正常

    // 右偏
    IR_R1 = 1,
    IR_R2 = 2,
    IR_R3 = 3,
    IR_R4 = 4,

    // 左偏
    IR_L1 = -1,
    IR_L2 = -2,
    IR_L3 = -3,
    IR_L4 = -4,

    IR_CORNER_R = 127, // 右转角
    IR_CORNER_L = 126, // 左转角
} IR_STATUS;

extern IR_STATUS g_ir_status;
extern const IR_STATUS IR_LUT[32];

extern int32_t g_ir_out;

void IR_Control(void);

/**
 * @brief 获取红外状态
 */
__STATIC_INLINE void IR_GetStatus(void)
{
    uint32_t port_val = LL_GPIO_ReadInputPort(IR1_GPIO_Port);
    uint32_t ir_val = (port_val & 0x0007u) | ((port_val >> 7) & 0x0018u);
    g_ir_status = IR_LUT[(~ir_val & 0x001Fu)];
}

/**
 * @brief 判断是否丢线
 * 
 * @return 丢线则返回1
 */
__STATIC_INLINE uint32_t IsLineLost(void)
{
    return (g_ir_status == IR_LOST ? 1 : 0);
}
