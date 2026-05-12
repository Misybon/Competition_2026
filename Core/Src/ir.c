#include "ir.h"
#include "config.h"
#include "main.h"
#include "track.h"

IR_STATUS g_ir_status = 0; // 红外状态值

// 红外状态查找表
const IR_STATUS IR_LUT[32] = {
    IR_LOST,
    IR_L4,
    IR_L2,
    IR_L3,
    IR_OK,
    IR_ERR,
    IR_L1,
    IR_CORNER_R,
    IR_R2,
    IR_ERR,
    IR_ERR,
    IR_ERR,
    IR_R1,
    IR_ERR,
    IR_OK,
    IR_CORNER_R,
    IR_R4,
    IR_ERR,
    IR_ERR,
    IR_ERR,
    IR_ERR,
    IR_ERR,
    IR_ERR,
    IR_ERR,
    IR_R3,
    IR_ERR,
    IR_ERR,
    IR_ERR,
    IR_CORNER_R,
    IR_ERR,
    IR_CORNER_R,
    IR_OK,
};

int32_t g_ir_out = 0;

/**
 * @brief 红外循迹控制
 */
void IR_Control(void)
{
    switch (g_ir_status)
    {
    case IR_ERR: // 红外值错误
        Track_Break();
        break;
    case IR_LOST: // 丢线
        break;
    case IR_CORNER_L: // 左转角
        break;
    case IR_CORNER_R: // 右转角
        break;
    default: // 正常情况
        g_track_speed.vy = g_ir_status * IR_VY_KP;
        g_track_speed.vz = g_ir_status * IR_VZ_KP;
        break;
    }
}
