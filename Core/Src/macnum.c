#include "macnum.h"
#include <stdlib.h>
#include "config.h"
#include "motor.h"

/**
 * @brief 麦轮运动学解算
 * 
 * @param Vx X轴方向速度，单位m/s
 * @param Vy Y轴方向速度，单位m/s
 * @param Vz 旋转速度，逆时针为正，单位rad/s
 */
void Move_Transform(int32_t Vx, int32_t Vy, int32_t Vz)
{
    // 单位m/s
    g_motor_tgtspeed._1 = Vx + Vy - Vz * ROT_PARAM / 100000;
    g_motor_tgtspeed._2 = Vx - Vy - Vz * ROT_PARAM / 100000;
    g_motor_tgtspeed._3 = Vx + Vy + Vz * ROT_PARAM / 100000;
    g_motor_tgtspeed._4 = Vx - Vy + Vz * ROT_PARAM / 100000;

    // 换算到PWM占空比
    g_motor_tgtspeed._1 *= MOTOR_KP;
    g_motor_tgtspeed._2 *= MOTOR_KP;
    g_motor_tgtspeed._3 *= MOTOR_KP;
    g_motor_tgtspeed._4 *= MOTOR_KP;

    // 目标值限幅
    uint32_t tmp_max_speed;

    // 取绝对值
    struct Motor_TargetSpeed tmp_motor_speed;
    tmp_motor_speed._1 = abs(g_motor_tgtspeed._1);
    tmp_motor_speed._2 = abs(g_motor_tgtspeed._2);
    tmp_motor_speed._3 = abs(g_motor_tgtspeed._3);
    tmp_motor_speed._4 = abs(g_motor_tgtspeed._4);

    // 线性求四路中的最大值
    tmp_max_speed = tmp_motor_speed._1;
    if (tmp_motor_speed._2 > tmp_max_speed)
    {
        tmp_max_speed = tmp_motor_speed._2;
    }
    if (tmp_motor_speed._3 > tmp_max_speed)
    {
        tmp_max_speed = tmp_motor_speed._3;
    }
    if (tmp_motor_speed._4 > tmp_max_speed)
    {
        tmp_max_speed = tmp_motor_speed._4;
    }

    // 按比例限幅
    if (tmp_max_speed > MOTOR_MAX_SPEED)
    {
        g_motor_tgtspeed._1 = g_motor_tgtspeed._1 * MOTOR_MAX_SPEED / tmp_max_speed;
        g_motor_tgtspeed._2 = g_motor_tgtspeed._2 * MOTOR_MAX_SPEED / tmp_max_speed;
        g_motor_tgtspeed._3 = g_motor_tgtspeed._3 * MOTOR_MAX_SPEED / tmp_max_speed;
        g_motor_tgtspeed._4 = g_motor_tgtspeed._4 * MOTOR_MAX_SPEED / tmp_max_speed;
    }
}
