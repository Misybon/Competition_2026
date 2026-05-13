#include "ir.h"
#include "config.h"
#include "main.h"
#include "state_handler.h"
#include "track.h"

extern volatile bool g_status_errorflag; // 状态机错误标志位
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
    IR_STOP,
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
    IR_ERR,
};

/**
 * @brief 红外循迹控制
 */
void IR_Control(void)
{
    switch (g_ir_status)
    {
    case IR_ERR: // 红外值错误
        Track_Break();
        g_status_errorflag = 1;
        Error_Handler(); // 进入错误处理
        break;
    case IR_LOST: // 丢线
        break;
    case IR_CORNER_L: // 左转角
        g_status = CORNER;
        break;
    case IR_CORNER_R: // 右转角
        g_status = CORNER;
        break;
    case IR_STOP: // 到达投掷区（旁路保证）
        Track_Break(); // 制动
        TIM6_Stop(); // 关闭定时器6
        Color_Stop(); // 关闭颜色传感器
        g_corner_count = 0; // 重置转弯计数
        g_status = THROW_PREPARE; // 进入投掷准备状态
        break;
    default: // 正常情况
        g_track_speed.vy = g_ir_status * IR_VY_KP; // 根据比例赋予平移速度以修正位置
        g_track_speed.vz = g_ir_status * IR_VZ_KP; // 根据比例赋予角速度以修正方向
        break;
    }
}
