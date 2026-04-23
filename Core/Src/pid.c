// 等待完善...

// 注意到电机离线时，特定情况下Out输出会出现严重问题，需要根据实际情况考虑是否修复...

#include "pid.h"
#include <math.h>
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
    float kp;
    float ki;
    float kd;
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
    float _1;
    float _2;
    float _3;
    float _4;
};

static struct Motor_PID s_motor_pid = { 0 };

static struct Motor_PID_Err s_motor_err_0 = { 0 };
static struct Motor_PID_Err s_motor_err_1 = { 0 };
static struct Motor_PID_Err s_motor_err_2 = { 0 };

static struct Motor_PID_Out s_motor_speed_lpf = { 0 };

static uint8_t s_motor_speed_lpf_inited = 0;
static uint8_t s_motor_ctrl_divider = 1;

struct Motor_PID_Out g_motor_out = { 0 };

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

    s_motor_err_0 = (struct Motor_PID_Err) { 0 };
    s_motor_err_1 = (struct Motor_PID_Err) { 0 };
    s_motor_err_2 = (struct Motor_PID_Err) { 0 };
    s_motor_speed_lpf = (struct Motor_PID_Out) { 0 };
    s_motor_speed_lpf_inited = 0;
    s_motor_ctrl_divider = 1;
    g_motor_out = (struct Motor_PID_Out) { 0 };
}

/**
 * @brief 根据目标速度修改PID分频值
 */
uint32_t PID_GetDivRatioByTargetSpeed(void)
{
    uint32_t abs_tgt_1 = abs(g_motor_tgtspeed._1);
    uint32_t abs_tgt_2 = abs(g_motor_tgtspeed._2);
    uint32_t abs_tgt_3 = abs(g_motor_tgtspeed._3);
    uint32_t abs_tgt_4 = abs(g_motor_tgtspeed._4);
    uint32_t abs_tgt_max = abs_tgt_1;

    if (abs_tgt_2 > abs_tgt_max)
    {
        abs_tgt_max = abs_tgt_2;
    }
    if (abs_tgt_3 > abs_tgt_max)
    {
        abs_tgt_max = abs_tgt_3;
    }
    if (abs_tgt_4 > abs_tgt_max)
    {
        abs_tgt_max = abs_tgt_4;
    }

    if (abs_tgt_max <= PID_DIV_SPEED_LOW)
    {
        return PID_DIV_RATIO_LOW;
    }
    if (abs_tgt_max <= PID_DIV_SPEED_MID)
    {
        return PID_DIV_RATIO_MID;
    }

    return PID_DIV_RATIO_HIGH;
}

/**
 * @brief 设置当前PID控制分频
 *
 * @param divider 分频值，最小为1
 */
void PID_SetControlDivider(uint32_t divider)
{
    if (divider == 0)
    {
        s_motor_ctrl_divider = 1;
        return;
    }

    s_motor_ctrl_divider = divider;
}

/**
 * @brief 红外PID控制
 * 
 */
void IR_PID_Control(void)
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

/**
 * @brief 电机和循迹PID控制
 */
void Motor_PID_Control(void)
{
    // 获取目标速度
    Move_Transform(g_track_speed.vx, g_track_speed.vy, g_track_speed.vz);

    // 获取当前速度
    {
        // 乘上分频值
        float speed_scale = MOTOR_ENCODER_KP / s_motor_ctrl_divider;
        float motor_speed_raw_1 = Motor_GetSpeed(MOTOR_ENCODER_1) * speed_scale;
        float motor_speed_raw_2 = Motor_GetSpeed(MOTOR_ENCODER_2) * speed_scale;
        float motor_speed_raw_3 = Motor_GetSpeed(MOTOR_ENCODER_3) * speed_scale;
        float motor_speed_raw_4 = Motor_GetSpeed(MOTOR_ENCODER_4) * speed_scale;

        // 一阶低通滤波
        if (!s_motor_speed_lpf_inited)
        {
            s_motor_speed_lpf._1 = motor_speed_raw_1;
            s_motor_speed_lpf._2 = motor_speed_raw_2;
            s_motor_speed_lpf._3 = motor_speed_raw_3;
            s_motor_speed_lpf._4 = motor_speed_raw_4;
            s_motor_speed_lpf_inited = 1;
        }
        else
        {
            s_motor_speed_lpf._1 += MOTOR_SPEED_LPF_ALPHA * (motor_speed_raw_1 - s_motor_speed_lpf._1);
            s_motor_speed_lpf._2 += MOTOR_SPEED_LPF_ALPHA * (motor_speed_raw_2 - s_motor_speed_lpf._2);
            s_motor_speed_lpf._3 += MOTOR_SPEED_LPF_ALPHA * (motor_speed_raw_3 - s_motor_speed_lpf._3);
            s_motor_speed_lpf._4 += MOTOR_SPEED_LPF_ALPHA * (motor_speed_raw_4 - s_motor_speed_lpf._4);
        }

        g_motor_speed._1 = (int32_t)(s_motor_speed_lpf._1);
        g_motor_speed._2 = (int32_t)(s_motor_speed_lpf._2);
        g_motor_speed._3 = (int32_t)(s_motor_speed_lpf._3);
        g_motor_speed._4 = (int32_t)(s_motor_speed_lpf._4);
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
        g_motor_out._1 += s_motor_pid.kp * (s_motor_err_0._1 - s_motor_err_1._1) + s_motor_pid.ki * s_motor_err_0._1 + s_motor_pid.kd * (s_motor_err_0._1 - (s_motor_err_1._1 * 2) + s_motor_err_2._1);
        g_motor_out._2 += s_motor_pid.kp * (s_motor_err_0._2 - s_motor_err_1._2) + s_motor_pid.ki * s_motor_err_0._2 + s_motor_pid.kd * (s_motor_err_0._2 - (s_motor_err_1._2 * 2) + s_motor_err_2._2);
        g_motor_out._3 += s_motor_pid.kp * (s_motor_err_0._3 - s_motor_err_1._3) + s_motor_pid.ki * s_motor_err_0._3 + s_motor_pid.kd * (s_motor_err_0._3 - (s_motor_err_1._3 * 2) + s_motor_err_2._3);
        g_motor_out._4 += s_motor_pid.kp * (s_motor_err_0._4 - s_motor_err_1._4) + s_motor_pid.ki * s_motor_err_0._4 + s_motor_pid.kd * (s_motor_err_0._4 - (s_motor_err_1._4 * 2) + s_motor_err_2._4);
    }

    // 输出限幅
    {
        if (g_motor_out._1 > MOTOR_MAX_SPEED)
        {
            g_motor_out._1 = MOTOR_MAX_SPEED;
        }
        else if (g_motor_out._1 < -MOTOR_MAX_SPEED)
        {
            g_motor_out._1 = -MOTOR_MAX_SPEED;
        }

        if (g_motor_out._2 > MOTOR_MAX_SPEED)
        {
            g_motor_out._2 = MOTOR_MAX_SPEED;
        }
        else if (g_motor_out._2 < -MOTOR_MAX_SPEED)
        {
            g_motor_out._2 = -MOTOR_MAX_SPEED;
        }

        if (g_motor_out._3 > MOTOR_MAX_SPEED)
        {
            g_motor_out._3 = MOTOR_MAX_SPEED;
        }
        else if (g_motor_out._3 < -MOTOR_MAX_SPEED)
        {
            g_motor_out._3 = -MOTOR_MAX_SPEED;
        }

        if (g_motor_out._4 > MOTOR_MAX_SPEED)
        {
            g_motor_out._4 = MOTOR_MAX_SPEED;
        }
        else if (g_motor_out._4 < -MOTOR_MAX_SPEED)
        {
            g_motor_out._4 = -MOTOR_MAX_SPEED;
        }
    }

    // 输出死区
    {
        if (fabs(g_motor_out._1) <= MOTOR_OUT_DEADZONE)
        {
            g_motor_out._1 = 0;
        }
        if (fabs(g_motor_out._2) <= MOTOR_OUT_DEADZONE)
        {
            g_motor_out._2 = 0;
        }
        if (fabs(g_motor_out._3) <= MOTOR_OUT_DEADZONE)
        {
            g_motor_out._3 = 0;
        }
        if (fabs(g_motor_out._4) <= MOTOR_OUT_DEADZONE)
        {
            g_motor_out._4 = 0;
        }
    }

    // 设置电机旋转方向
    {
        // 电机1
        if (g_motor_out._1 > 0)
        {
            LL_GPIO_SetOutputPin(Motor1_Con1_GPIO_Port, Motor1_Con1_Pin);
            LL_GPIO_ResetOutputPin(Motor1_Con2_GPIO_Port, Motor1_Con2_Pin);
        }
        else if (g_motor_out._1 < 0)
        {
            LL_GPIO_ResetOutputPin(Motor1_Con1_GPIO_Port, Motor1_Con1_Pin);
            LL_GPIO_SetOutputPin(Motor1_Con2_GPIO_Port, Motor1_Con2_Pin);
        }
        else
        {
            LL_GPIO_SetOutputPin(Motor1_Con1_GPIO_Port, Motor1_Con1_Pin);
            LL_GPIO_SetOutputPin(Motor1_Con2_GPIO_Port, Motor1_Con2_Pin);
        }

        // 电机2
        if (g_motor_out._2 > 0)
        {
            LL_GPIO_SetOutputPin(Motor2_Con1_GPIO_Port, Motor2_Con1_Pin);
            LL_GPIO_ResetOutputPin(Motor2_Con2_GPIO_Port, Motor2_Con2_Pin);
        }
        else if (g_motor_out._2 < 0)
        {
            LL_GPIO_ResetOutputPin(Motor2_Con1_GPIO_Port, Motor2_Con1_Pin);
            LL_GPIO_SetOutputPin(Motor2_Con2_GPIO_Port, Motor2_Con2_Pin);
        }
        else
        {
            LL_GPIO_SetOutputPin(Motor2_Con1_GPIO_Port, Motor2_Con1_Pin);
            LL_GPIO_SetOutputPin(Motor2_Con2_GPIO_Port, Motor2_Con2_Pin);
        }

        // 电机3
        if (g_motor_out._3 > 0)
        {
            LL_GPIO_SetOutputPin(Motor3_Con1_GPIO_Port, Motor3_Con1_Pin);
            LL_GPIO_ResetOutputPin(Motor3_Con2_GPIO_Port, Motor3_Con2_Pin);
        }
        else if (g_motor_out._3 < 0)
        {
            LL_GPIO_ResetOutputPin(Motor3_Con1_GPIO_Port, Motor3_Con1_Pin);
            LL_GPIO_SetOutputPin(Motor3_Con2_GPIO_Port, Motor3_Con2_Pin);
        }
        else
        {
            LL_GPIO_SetOutputPin(Motor3_Con1_GPIO_Port, Motor3_Con1_Pin);
            LL_GPIO_SetOutputPin(Motor3_Con2_GPIO_Port, Motor3_Con2_Pin);
        }

        // 电机4
        if (g_motor_out._4 > 0)
        {
            LL_GPIO_SetOutputPin(Motor4_Con1_GPIO_Port, Motor4_Con1_Pin);
            LL_GPIO_ResetOutputPin(Motor4_Con2_GPIO_Port, Motor4_Con2_Pin);
        }
        else if (g_motor_out._4 < 0)
        {
            LL_GPIO_ResetOutputPin(Motor4_Con1_GPIO_Port, Motor4_Con1_Pin);
            LL_GPIO_SetOutputPin(Motor4_Con2_GPIO_Port, Motor4_Con2_Pin);
        }
        else
        {
            LL_GPIO_SetOutputPin(Motor4_Con1_GPIO_Port, Motor4_Con1_Pin);
            LL_GPIO_SetOutputPin(Motor4_Con2_GPIO_Port, Motor4_Con2_Pin);
        }
    }

    // 输出速度
    {
        Motor_SetSpeed(MOTOR_1, fabs(g_motor_out._1));
        Motor_SetSpeed(MOTOR_2, fabs(g_motor_out._2));
        Motor_SetSpeed(MOTOR_3, fabs(g_motor_out._3));
        Motor_SetSpeed(MOTOR_4, fabs(g_motor_out._4));
    }
}
