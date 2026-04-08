// 等待完善...

#pragma once

#include "main.h"
#include "motor.h"

extern struct Track_Speed g_track_speed;

/**
 * @brief 麦轮运动学解算
 * 
 * @param Vx X轴方向速度，单位m/s
 * @param Vy Y轴方向速度，单位m/s
 * @param Vz 旋转速度，逆时针为正，单位rad/s
 */
__STATIC_INLINE void Move_Transform(int32_t Vx, int32_t Vy, int32_t Vz)
{
    g_motor_tgtspeed._1 = Vx + Vy - Vz * ROT_PARAM;
    g_motor_tgtspeed._2 = Vx - Vy - Vz * ROT_PARAM;
    g_motor_tgtspeed._3 = Vx + Vy + Vz * ROT_PARAM;
    g_motor_tgtspeed._4 = Vx - Vy + Vz * ROT_PARAM;
}
