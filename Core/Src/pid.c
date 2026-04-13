// 等待完善...

#include "pid.h"
#include <stdlib.h>
#include "macnum.h"
#include "main.h"
#include "track.h"

struct IR_PID
{
    uint16_t kp;
    uint16_t ki;
    uint16_t kd;
};

static int32_t s_ir_current = 0;
static int32_t s_ir_err_0 = 0;
static int32_t s_ir_err_1 = 0;
static int32_t s_ir_err_2 = 0;

static struct IR_PID s_ir_pid = { 0 };

struct Motor_PID
{
    uint16_t kp;
    uint16_t ki;
    uint16_t kd;
};

struct Motor_PID_Err
{
    int32_t _1;
    int32_t _2;
    int32_t _3;
    int32_t _4;
};

struct Motor_PID_Out
{
    int32_t _1;
    int32_t _2;
    int32_t _3;
    int32_t _4;
};

static struct Motor_PID s_motor_pid = { 0 };

static struct Motor_PID_Err s_motor_err_0 = { 0 };
static struct Motor_PID_Err s_motor_err_1 = { 0 };
static struct Motor_PID_Err s_motor_err_2 = { 0 };

static struct Motor_PID_Out s_motor_out = { 0 };

/**
 * @brief PID初始化
 */
void PID_Init(void)
{
    s_ir_pid.kp = IR_PID_KP;
    s_ir_pid.ki = IR_PID_KI;
    s_ir_pid.kd = IR_PID_KD;

    s_motor_pid.kp = MOTOR_PID_KP;
    s_motor_pid.ki = MOTOR_PID_KI;
    s_motor_pid.kd = MOTOR_PID_KD;
}

/**
 * @brief 电机和循迹PID控制
 */
void PID_Control(void)
{
    // 红外PID
    {
        // 获取当前值
        s_ir_current = -IR_WEIGH_1 * g_ir_val._1 - IR_WEIGH_2 * g_ir_val._2 + IR_WEIGH_3 * g_ir_val._3 + IR_WEIGH_2 * g_ir_val._4 + IR_WEIGH_1 * g_ir_val._5;

        // 计算误差值
        s_ir_err_2 = s_ir_err_1;
        s_ir_err_1 = s_ir_err_0;
        s_ir_err_0 = IR_TGT - s_ir_current;

        // 将放大的输出值按比例减小
        int32_t ir_delta = s_ir_pid.kp * (s_ir_err_0 - s_ir_err_1) + s_ir_pid.ki * s_ir_err_0 + s_ir_pid.kd * (s_ir_err_0 - (s_ir_err_1 * 2) + s_ir_err_2);
        g_ir_out += (ir_delta >> 8);

        // 根据输出值修改角速度
        g_track_speed.vz = IR_KP * g_ir_out;
    }

    // 电机PID
    {
        // 获取目标速度
        Move_Transform(g_track_speed.vx, g_track_speed.vy, g_track_speed.vz);
        // 乘上比例
        g_motor_speed._1 = Motor_GetSpeed(MOTOR_ENCODER_1) * MOTOR_ENCODER_KP;
        g_motor_speed._2 = Motor_GetSpeed(MOTOR_ENCODER_2) * MOTOR_ENCODER_KP;
        g_motor_speed._3 = Motor_GetSpeed(MOTOR_ENCODER_3) * MOTOR_ENCODER_KP;
        g_motor_speed._4 = Motor_GetSpeed(MOTOR_ENCODER_4) * MOTOR_ENCODER_KP;
    }

    // 计算误差值
    {
        s_motor_err_2 = s_motor_err_1;
        s_motor_err_1 = s_motor_err_0;

        s_motor_err_0._1 = g_motor_tgtspeed._1 - g_motor_speed._1;
        s_motor_err_0._2 = g_motor_tgtspeed._2 - g_motor_speed._2;
        s_motor_err_0._3 = g_motor_tgtspeed._3 - g_motor_speed._3;
        s_motor_err_0._4 = g_motor_tgtspeed._4 - g_motor_speed._4;
    }

    // 计算输出值
    {
        int32_t motor_delta_1 = s_motor_pid.kp * (s_motor_err_0._1 - s_motor_err_1._1) + s_motor_pid.ki * s_motor_err_0._1 + s_motor_pid.kd * (s_motor_err_0._1 - (s_motor_err_1._1 * 2) + s_motor_err_2._1);
        int32_t motor_delta_2 = s_motor_pid.kp * (s_motor_err_0._2 - s_motor_err_1._2) + s_motor_pid.ki * s_motor_err_0._2 + s_motor_pid.kd * (s_motor_err_0._2 - (s_motor_err_1._2 * 2) + s_motor_err_2._2);
        int32_t motor_delta_3 = s_motor_pid.kp * (s_motor_err_0._3 - s_motor_err_1._3) + s_motor_pid.ki * s_motor_err_0._3 + s_motor_pid.kd * (s_motor_err_0._3 - (s_motor_err_1._3 * 2) + s_motor_err_2._3);
        int32_t motor_delta_4 = s_motor_pid.kp * (s_motor_err_0._4 - s_motor_err_1._4) + s_motor_pid.ki * s_motor_err_0._4 + s_motor_pid.kd * (s_motor_err_0._4 - (s_motor_err_1._4 * 2) + s_motor_err_2._4);

        // 输出值按比例缩小
        s_motor_out._1 += (motor_delta_1 >> 8);
        s_motor_out._2 += (motor_delta_2 >> 8);
        s_motor_out._3 += (motor_delta_3 >> 8);
        s_motor_out._4 += (motor_delta_4 >> 8);
    }

    // 输出限幅
    {
        if (s_motor_out._1 > MOTOR_MAX_SPEED)
        {
            s_motor_out._1 = MOTOR_MAX_SPEED;
        }
        else if (s_motor_out._1 < -MOTOR_MAX_SPEED)
        {
            s_motor_out._1 = -MOTOR_MAX_SPEED;
        }
        if (s_motor_out._2 > MOTOR_MAX_SPEED)
        {
            s_motor_out._2 = MOTOR_MAX_SPEED;
        }
        else if (s_motor_out._2 < -MOTOR_MAX_SPEED)
        {
            s_motor_out._2 = -MOTOR_MAX_SPEED;
        }
        if (s_motor_out._3 > MOTOR_MAX_SPEED)
        {
            s_motor_out._3 = MOTOR_MAX_SPEED;
        }
        else if (s_motor_out._3 < -MOTOR_MAX_SPEED)
        {
            s_motor_out._3 = -MOTOR_MAX_SPEED;
        }
        if (s_motor_out._4 > MOTOR_MAX_SPEED)
        {
            s_motor_out._4 = MOTOR_MAX_SPEED;
        }
        else if (s_motor_out._4 < -MOTOR_MAX_SPEED)
        {
            s_motor_out._4 = -MOTOR_MAX_SPEED;
        }
    }

    // 设置电机旋转方向
    {
        // 电机1
        if (s_motor_out._1 >= 0)
        {
            LL_GPIO_ResetOutputPin(Motor1_Con1_GPIO_Port, Motor1_Con1_Pin);
            LL_GPIO_SetOutputPin(Motor1_Con2_GPIO_Port, Motor1_Con2_Pin);
        }
        else
        {
            LL_GPIO_SetOutputPin(Motor1_Con1_GPIO_Port, Motor1_Con1_Pin);
            LL_GPIO_ResetOutputPin(Motor1_Con2_GPIO_Port, Motor1_Con2_Pin);
        }

        // 电机2
        if (s_motor_out._2 >= 0)
        {
            LL_GPIO_ResetOutputPin(Motor2_Con1_GPIO_Port, Motor2_Con1_Pin);
            LL_GPIO_SetOutputPin(Motor2_Con2_GPIO_Port, Motor2_Con2_Pin);
        }
        else
        {
            LL_GPIO_SetOutputPin(Motor2_Con1_GPIO_Port, Motor2_Con1_Pin);
            LL_GPIO_ResetOutputPin(Motor2_Con2_GPIO_Port, Motor2_Con2_Pin);
        }

        // 电机3
        if (s_motor_out._3 >= 0)
        {
            LL_GPIO_ResetOutputPin(Motor3_Con1_GPIO_Port, Motor3_Con1_Pin);
            LL_GPIO_SetOutputPin(Motor3_Con2_GPIO_Port, Motor3_Con2_Pin);
        }
        else
        {
            LL_GPIO_SetOutputPin(Motor3_Con1_GPIO_Port, Motor3_Con1_Pin);
            LL_GPIO_ResetOutputPin(Motor3_Con2_GPIO_Port, Motor3_Con2_Pin);
        }

        // 电机4
        if (s_motor_out._4 >= 0)
        {
            LL_GPIO_ResetOutputPin(Motor4_Con1_GPIO_Port, Motor4_Con1_Pin);
            LL_GPIO_SetOutputPin(Motor4_Con2_GPIO_Port, Motor4_Con2_Pin);
        }
        else
        {
            LL_GPIO_SetOutputPin(Motor4_Con1_GPIO_Port, Motor4_Con1_Pin);
            LL_GPIO_ResetOutputPin(Motor4_Con2_GPIO_Port, Motor4_Con2_Pin);
        }
    }

    // 输出速度
    {
        Motor_SetSpeed(MOTOR_1, abs(s_motor_out._1));
        Motor_SetSpeed(MOTOR_2, abs(s_motor_out._2));
        Motor_SetSpeed(MOTOR_3, abs(s_motor_out._3));
        Motor_SetSpeed(MOTOR_4, abs(s_motor_out._4));
    }
}
