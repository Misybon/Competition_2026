// 等待完善...

#pragma once

#include "main.h"

struct Track_Speed
{
    int32_t vx;
    int32_t vy;
    int32_t vz;
};

extern struct Track_Speed g_track_speed;

// 待完善...
// 应该在函数内加入IN引脚控制
__STATIC_INLINE void Motor_Rot_Angle(int32_t Angle)
{
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

void Track_Start(void);
void Track_Stop(void);
void Track_Break(void);
void Track_Restart(void);
void ProcessLineLostEvent(void);
