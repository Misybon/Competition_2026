// 等待完善...

#pragma once

#include "ir.h"
#include "main.h"

struct Track_Speed
{
    int32_t vx;
    int32_t vy;
    int32_t vz;
};

extern struct Track_Speed g_track_speed;

/**
 * @brief 小车旋转指定角度，逆时针正，顺时针负
 * 
 * @param Angle 旋转的角度，应该只有90, -90, 180
 */
__STATIC_INLINE void Motor_Rot_Angle(int32_t Angle)
{
    if (Angle == 90 || Angle == -90)
    {
        g_track_speed.vz = 500;
        while (g_ir_val._3) // 等待中间红外传感器识别到黑线
        {
        }
        g_track_speed.vz = 0;
    }
    else if (Angle == 180)
    {
        // 考虑是否使用视觉识别中...
    }
    else
    {
        return;
    }
}

/**
 * @brief 让电机以给定速度旋转
 * 
 * @param Speed 旋转速度，逆时针为正
 */
__STATIC_INLINE void Motor_Rot_Speed(int32_t Speed)
{
    g_track_speed.vz = Speed;
}

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

    for (uint32_t speed = 0; speed <= 999; speed++) // 坡度启动
    {
        g_track_speed.vx = speed;
        LL_mDelay(1);
    }
}

/**
 * @brief 整车刹车
 */
__STATIC_INLINE void Track_Break(void)
{
    // 是否还要下面四个函数有待考量...
    // Motor1_Break();
    // Motor2_Break();
    // Motor3_Break();
    // Motor4_Break();

    g_track_speed.vx = g_track_speed.vy = g_track_speed.vz = 0;
}

void Track_Start(void);
void Track_Stop(void);
// void Track_Break(void);
// void Track_Restart(void);
void ProcessLineLostEvent(void);
