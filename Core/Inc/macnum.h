#ifndef __MACNUM_H
#define __MACNUM_H

#include "main.h"
#include "motor.h"

#define CAR_H 0 // 底盘轮距
#define CAR_W 0 // 底盘轴距
#define ROT_PARAM ((CAR_H >> 1) + (CAR_W >> 1)) // 旋转参数

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

#endif
