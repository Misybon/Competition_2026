// 等待完善...

#include "track.h"
#include "main.h"
#include "motor.h"
#include "stm32f1xx_hal.h"

volatile bool g_break_flag = 0; // 制动状态标志位

extern volatile bool g_motor_startflag;
extern volatile bool g_status_errorflag;

struct Track_Speed g_track_speed = { 0 };

/**
 * @brief 开始循迹
 */
void Track_Start(void)
{
    Motor1_Start();
    Motor2_Start();
    Motor3_Start();
    Motor4_Start();
    for (uint32_t speed = 0; speed <= MAX_VX; speed++) // 坡度启动
    {
        g_track_speed.vx = speed;
        LL_mDelay(1);
    }
    g_motor_startflag = 1;
}

/**
 * @brief 停止循迹
 */
void Track_Stop(void)
{
    Motor_Stop(MOTOR_1);
    Motor_Stop(MOTOR_2);
    Motor_Stop(MOTOR_3);
    Motor_Stop(MOTOR_4);
    g_motor_startflag = 0;
}

/**
 * @brief 整车刹车
 */
void Track_Break(void)
{
    Motor1_Break();
    Motor2_Break();
    Motor3_Break();
    Motor4_Break();

    g_break_flag = 1;
    g_track_speed.vx = g_track_speed.vy = g_track_speed.vz = 0;
}

/**
 * @brief 小车旋转指定角度，逆时针正，顺时针负
 * 
 * @param Angle 旋转的角度，应该只有90, -90, 180
 */
void Track_Rot_Angle(int32_t Angle)
{
    if (Angle == 90)
    {
        g_track_speed.vz = MAX_VZ; // 最大速度逆时针旋转
        while (g_ir_val._2) // 等待红外传感器识别到黑线
        {
        }
        Track_Break_Soft(); // 制动
    }
    else if (Angle == -90)
    {
        g_track_speed.vz = -MAX_VZ; // 最大速度顺时针旋转
        while (g_ir_val._4) // 等待红外传感器识别到黑线
        {
        }
        Track_Break_Soft(); // 制动
    }
    else if (Angle == 180)
    {
        g_track_speed.vz = MAX_VZ;
        while (g_ir_val._3) // 等待中间红外传感器识别到黑线
        {
        }
        Track_Break();
    }
    else
    {
        return;
    }
}

// /**
//  * @brief 重启循迹
//  */
// void Track_Restart(void)
// {
//     // 是否还要下面四个函数有待考量...
//     // Motor1_Restart();
//     // Motor2_Restart();
//     // Motor3_Restart();
//     // Motor4_Restart();

//     for (uint32_t speed = 0; speed <= 999; speed++) // 坡度启动
//     {
//         g_track_speed.vx = speed;
//         LL_mDelay(1);
//     }
// }

/**
 * @brief 处理丢线事件
 */
void ProcessLineLostEvent(void)
{
    // 时间阈值判断
    uint32_t tick_start = HAL_GetTick();
    while (HAL_GetTick() - tick_start < LINELOST_TIME)
    {
        IR_GetVal();

        if (!IsLineLost())
        {
            return; // 没丢线就返回
        }
    }

    // 丢线了
    if (g_corner_count == 0) // 第一次丢线
    {
        g_status = CORNER; // 进入转弯状态
    }
    else if (g_corner_count == 1) // 第二次丢线
    {
        if (!g_return_flag) // 不处于返回状态
        {
            // 直接在此完成转弯动作
            Track_Break_Soft(); // 软制动
            Track_Rot_Angle(-90); // 顺时针旋转90°
            Track_Restart(); // 重启循迹

            g_corner_count++; // 转弯计数+1
            g_status = THROW_PREPARE; // 等待进入投掷区
        }
        else // 处于返回状态
        {
            // 直接在此完成转弯动作
            Track_Break_Soft(); // 软制动
            Track_Rot_Angle(90); // 逆时针旋转90°
            Track_Restart(); // 重启循迹

            g_corner_count++; // 转弯计数+1
            g_status = STOP_PREPARE; // 准备停车
        }
    }
    else if (g_corner_count >= 2) // 第三次丢线
    {
        // 出现问题！！！
        g_corner_count = 0; // 重置转弯计数
        g_motor_startflag = 0; // 重置电机启动状态
        g_status_errorflag = 1; // 状态机错误
        Error_Handler(); // 进入错误处理，尝试恢复待机模式
    }
}
