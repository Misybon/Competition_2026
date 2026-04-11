// 等待完善...

#include "track.h"
#include "motor.h"

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
    for (uint32_t speed = 0; speed <= 999; speed++) // 坡度启动
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
    while (g_break_flag) // 等待制动完成
    {
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
    IR_GetVal();

    if (!IsLineLost())
    {
        return; // 没丢线就返回
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
            g_status = CORNER; // 进入转弯状态
        }
        else // 处于返回状态
        {
            // 直接在此完成转弯动作
            Track_Break(); // 制动
            Track_Rot_Angle(-90); // 顺时针旋转90°
            Track_Restart(); // 重启循迹

            g_status = STOP_PREPARE; // 准备停车
        }
    }
    else if (g_corner_count >= 2) // 第三次丢线
    {
        if (!g_return_flag) // 不处于返回状态
        {
            g_status = THROW_PREPARE; // 准备进入投掷状态
        }
        else // 出现问题！！！
        {
            g_corner_count = 0; // 重置转弯计数
            g_motor_startflag = 0; // 重置电机启动状态
            g_status_errorflag = 1; // 状态机错误
            Error_Handler(); // 进入错误处理，尝试恢复待机模式
        }
    }
}
