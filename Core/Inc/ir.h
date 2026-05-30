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
    IR_R1 = 2,
    IR_R2 = 4,
    IR_R3 = 5,
    IR_R4 = 6,

    // 左偏
    IR_L1 = -2,
    IR_L2 = -4,
    IR_L3 = -5,
    IR_L4 = -6,

    IR_STOP = 125,
    IR_CORNER_R = 127, // 右转角
    IR_CORNER_L = 126, // 左转角
} IR_STATUS;

extern IR_STATUS g_ir_status;
extern const IR_STATUS IR_LUT[32];

void IR_Control(void);
void IR_GetStatus(void);

/**
 * @brief 判断是否丢线
 * 
 * @return 丢线则返回1
 */
__STATIC_INLINE uint32_t IsLineLost(void)
{
    return (g_ir_status == IR_LOST ? 1 : 0);
}
