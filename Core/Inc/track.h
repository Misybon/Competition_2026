// 等待完善...

#pragma once

#include "config.h"
#include "ir.h"
#include "motor.h"

struct Track_Speed
{
    int32_t vx;
    int32_t vy;
    int32_t vz;
};

extern struct Track_Speed g_track_speed;
extern volatile bool g_break_flag;

void Track_Start(void);
void Track_Stop(void);
void Track_Break(void);
// void Track_Restart(void);
void ProcessLineLostEvent(void);

/**
 * @brief 小车旋转指定角度，逆时针正，顺时针负
 * 
 * @param Angle 旋转的角度，应该只有90, -90, 180
 */
__STATIC_INLINE void Track_Rot_Angle(int32_t Angle)
{
    if (Angle == 90 || Angle == -90)
    {
        g_track_speed.vz = MAX_VZ;
        while (g_ir_val._3) // 等待中间红外传感器识别到黑线
        {
        }
        Track_Break();
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
//  * @brief 让电机以给定速度旋转
//  *
//  * @param Speed 旋转速度，逆时针为正
//  */
// __STATIC_INLINE void Track_Rot_Speed(int32_t Speed)
// {
//     g_track_speed.vz = Speed;
// }

/**
 * @brief 重启循迹
 */
__STATIC_INLINE void Track_Restart(void)
{
    // 是否还要下面四个函数有待考量...
    // Motor1_Restart();
    // Motor2_Restart();
    // Motor3_Restart();
    // Motor4_Restart();

    for (uint32_t speed = 0; speed <= MAX_VX; speed++) // 坡度启动
    {
        g_track_speed.vx = speed;
        LL_mDelay(1);
    }
}
