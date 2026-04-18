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
void Track_Rot_Angle(int32_t Angle);

/**
 * @brief 整车软制动(滑行)
 */
__STATIC_INLINE void Track_Break_Soft(void)
{
    g_track_speed.vx = 0;
    g_track_speed.vy = 0;
    g_track_speed.vz = 0;
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
    g_track_speed.vx = MAX_VX; // 无须坡度启动
}
