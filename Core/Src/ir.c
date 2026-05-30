#include "ir.h"
#include "config.h"
#include "main.h"
#include "state_handler.h"
#include "track.h"

extern volatile bool g_status_errorflag; // 状态机错误标志位
IR_STATUS g_ir_status = IR_OK; // 红外状态值
static IR_STATUS s_last_valid = IR_OK; // 上次有效值
static uint8_t s_lost_cnt = 0; // 丢线计数值

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
    IR_STOP,
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
    IR_STOP,
    IR_ERR,
};

/**
 * @brief 获取红外状态
 */
void IR_GetStatus(void)
{
    uint32_t port_val = LL_GPIO_ReadInputPort(IR1_GPIO_Port); // 读取端口值
    uint32_t ir_val = (port_val & 0x0007u) | ((port_val >> 7) & 0x0018u); // 获取红外值
    IR_STATUS cur = IR_LUT[(~ir_val & 0x001Fu)]; // 根据LUT获取红外状态

    if (cur == IR_LOST) // 丢线
    {
        if (s_lost_cnt < LINELOST_CNT) // 小于计数
        {
            s_lost_cnt++; // 计数值+1
            g_ir_status = s_last_valid; // 维持旧状态
        }
        else // 大于计数
        {
            g_ir_status = IR_LOST; // 更新状态为丢线
        }
        return; // 返回
    }

    s_lost_cnt = 0; // 清零计数
    g_ir_status = cur; // 更新状态
    if (cur != IR_ERR) // 红外未发生错误
    {
        s_last_valid = cur;
    }
}

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
        if (g_corner_count == 0) // 第一次
        {
            g_status = CORNER; // 进入转弯状态
        }
        else if (g_corner_count == 1) // 第二次
        {
            if (!g_return_flag) // 不处于返回状态
            {
                // 直接在此完成转弯动作
                Track_Break(); // 制动
                Track_Rot_Angle(-90); // 顺时针旋转90°
                Track_Restart(); // 重启循迹

                g_corner_count++; // 转弯计数+1
                g_status = THROW_PREPARE; // 等待进入投掷区
            }
            else // 处于返回状态
            {
                // 直接在此完成转弯动作
                Track_Break(); // 制动
                Track_Rot_Angle(90); // 逆时针旋转90°
                Track_Restart(); // 重启循迹

                g_corner_count++; // 转弯计数+1
                g_status = STOP_PREPARE; // 准备停车
            }
        }
        else if (g_corner_count >= 2) // 第三次
        {
            // 出现问题！！！
            g_corner_count = 0; // 重置转弯计数
            g_motor_startflag = 0; // 重置电机启动状态
            g_status_errorflag = 1; // 状态机错误
            Error_Handler(); // 进入错误处理，尝试恢复待机模式
        }
        break;
    case IR_CORNER_R: // 右转角
        if (g_corner_count == 0) // 第一次
        {
            g_status = CORNER; // 进入转弯状态
        }
        else if (g_corner_count == 1) // 第二次
        {
            if (!g_return_flag) // 不处于返回状态
            {
                // 直接在此完成转弯动作
                Track_Break(); // 制动
                Track_Rot_Angle(-90); // 顺时针旋转90°
                Track_Restart(); // 重启循迹

                g_corner_count++; // 转弯计数+1
                g_status = THROW_PREPARE; // 等待进入投掷区
            }
            else // 处于返回状态
            {
                // 直接在此完成转弯动作
                Track_Break(); // 制动
                Track_Rot_Angle(90); // 逆时针旋转90°
                Track_Restart(); // 重启循迹

                g_corner_count++; // 转弯计数+1
                g_status = STOP_PREPARE; // 准备停车
            }
        }
        else if (g_corner_count >= 2) // 第三次
        {
            // 出现问题！！！
            g_corner_count = 0; // 重置转弯计数
            g_motor_startflag = 0; // 重置电机启动状态
            g_status_errorflag = 1; // 状态机错误
            Error_Handler(); // 进入错误处理，尝试恢复待机模式
        }
        break;
    case IR_STOP: // 到达投掷区（旁路保证）
        Track_Break(); // 制动
        TIM6_Stop(); // 关闭定时器6
        Color_Stop(); // 关闭颜色传感器
        g_corner_count = 0; // 重置转弯计数
        g_status = THROW_PREPARE; // 进入投掷准备状态
        break;
    default: // 正常情况
        g_track_speed.vy = (g_ir_status * IR_VY_KP) * -1; // 根据比例赋予平移速度以修正位置
        g_track_speed.vz = (g_ir_status * IR_VZ_KP) * -1; // 根据比例赋予角速度以修正方向
        break;
    }
}
