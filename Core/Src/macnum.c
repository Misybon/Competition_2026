// 等待完善...

#include "macnum.h"
#include "main.h"
#include "motor.h"
#include "stdlib.h"

/**
 * @brief 麦轮运动学解算
 * 
 * @param Vx X轴方向速度，单位m/s
 * @param Vy Y轴方向速度，单位m/s
 * @param Vz 旋转速度，逆时针为正，单位rad/s
 */
void Move_Transform(int32_t Vx, int32_t Vy, int32_t Vz)
{
    g_motor_tgtspeed._1 = Vx + Vy - Vz * ROT_PARAM;
    g_motor_tgtspeed._2 = Vx - Vy - Vz * ROT_PARAM;
    g_motor_tgtspeed._3 = Vx + Vy + Vz * ROT_PARAM;
    g_motor_tgtspeed._4 = Vx - Vy + Vz * ROT_PARAM;

    // 目标值限幅
    uint32_t tmp_max_speed = 0;

    // 读取最大值
    if (abs(g_motor_tgtspeed._1) > MOTOR_MAX_SPEED)
    {
        tmp_max_speed = abs(g_motor_tgtspeed._1);
    }
    else if (abs(g_motor_tgtspeed._2) > MOTOR_MAX_SPEED)
    {
        tmp_max_speed = abs(g_motor_tgtspeed._2);
    }
    else if (abs(g_motor_tgtspeed._3) > MOTOR_MAX_SPEED)
    {
        tmp_max_speed = abs(g_motor_tgtspeed._3);
    }
    else if (abs(g_motor_tgtspeed._4) > MOTOR_MAX_SPEED)
    {
        tmp_max_speed = abs(g_motor_tgtspeed._4);
    }

    // 按比例限幅
    if (tmp_max_speed != 0)
    {
        uint32_t rate = tmp_max_speed / MOTOR_MAX_SPEED + 1;

        g_motor_tgtspeed._1 /= rate;
        g_motor_tgtspeed._2 /= rate;
        g_motor_tgtspeed._3 /= rate;
        g_motor_tgtspeed._4 /= rate;
    }
}
