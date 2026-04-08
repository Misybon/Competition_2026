// 等待完善...

#include "macnum.h"
#include "main.h"
#include "motor.h"
#include "stdlib.h"
#include "track.h"

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

    struct Motor_TargetSpeed tmp_motor_speed;
    tmp_motor_speed._1 = abs(g_motor_tgtspeed._1);
    tmp_motor_speed._2 = abs(g_motor_tgtspeed._2);
    tmp_motor_speed._3 = abs(g_motor_tgtspeed._3);
    tmp_motor_speed._4 = abs(g_motor_tgtspeed._4);

    // 判断是否溢出
    if (tmp_motor_speed._1 > MOTOR_MAX_SPEED || tmp_motor_speed._2 > MOTOR_MAX_SPEED || tmp_motor_speed._3 > MOTOR_MAX_SPEED || tmp_motor_speed._4 > MOTOR_MAX_SPEED)
    {
        // 读取最大值
        if (tmp_motor_speed._1 > tmp_motor_speed._2) // 1 > 2
        {
            if (tmp_motor_speed._3 > tmp_motor_speed._4) // 3 > 4
            {
                if (tmp_motor_speed._1 > tmp_motor_speed._3) // 1 > 3
                {
                    tmp_max_speed = tmp_motor_speed._1; // max = 1
                }
                else // 3 > 1
                {
                    tmp_max_speed = tmp_motor_speed._3; // max = 3
                }
            }
            else // 4 > 3
            {
                if (tmp_motor_speed._1 > tmp_motor_speed._4) // 1 > 4
                {
                    tmp_max_speed = tmp_motor_speed._1; // max = 1
                }
                else
                {
                    tmp_max_speed = tmp_motor_speed._4; /// max = 4
                }
            }
        }
        else // 2 > 1
        {
            if (tmp_motor_speed._3 > tmp_motor_speed._4) // 3 > 4
            {
                if (tmp_motor_speed._2 > tmp_motor_speed._3) // 2 > 3
                {
                    tmp_max_speed = tmp_motor_speed._2; // max = 2
                }
                else // 3 > 2
                {
                    tmp_max_speed = tmp_motor_speed._3; // max = 3
                }
            }
            else // 4 > 3
            {
                if (tmp_motor_speed._2 > tmp_motor_speed._4) // 2 > 4
                {
                    tmp_max_speed = tmp_motor_speed._2; // max = 2
                }
                else // 4 > 2
                {
                    tmp_max_speed = tmp_motor_speed._4; // max = 4
                }
            }
        }
    }

    // 按比例限幅
    if (tmp_max_speed)
    {
        g_motor_tgtspeed._1 = g_motor_tgtspeed._1 * MOTOR_MAX_SPEED / tmp_max_speed;
        g_motor_tgtspeed._2 = g_motor_tgtspeed._2 * MOTOR_MAX_SPEED / tmp_max_speed;
        g_motor_tgtspeed._3 = g_motor_tgtspeed._3 * MOTOR_MAX_SPEED / tmp_max_speed;
        g_motor_tgtspeed._4 = g_motor_tgtspeed._4 * MOTOR_MAX_SPEED / tmp_max_speed;
    }
}
