// 等待完善...

#include "pid.h"
#include "ir.h"
#include "main.h"
#include "motor.h"

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
    // 电机PID
    {
        // 获取目标速度
        Move_Transform(g_track_speed.vx, g_track_speed.vy, g_track_speed.vz);

        // 获取实际速度
        {
            g_motor_speed._1 = Motor_GetSpeed(MOTOR_ENCODER_1);
            g_motor_speed._2 = Motor_GetSpeed(MOTOR_ENCODER_2);
            g_motor_speed._3 = Motor_GetSpeed(MOTOR_ENCODER_3);
            g_motor_speed._4 = Motor_GetSpeed(MOTOR_ENCODER_4);
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
            s_motor_out._1 += s_motor_pid.kp * (s_motor_err_0._1 - s_motor_err_1._1) + s_motor_pid.ki * s_motor_err_0._1 + s_motor_pid.kd * (s_motor_err_0._1 - (s_motor_err_1._1 << 1) + s_motor_err_2._1);
            s_motor_out._2 += s_motor_pid.kp * (s_motor_err_0._2 - s_motor_err_1._2) + s_motor_pid.ki * s_motor_err_0._2 + s_motor_pid.kd * (s_motor_err_0._2 - (s_motor_err_1._2 << 1) + s_motor_err_2._2);
            s_motor_out._3 += s_motor_pid.kp * (s_motor_err_0._3 - s_motor_err_1._3) + s_motor_pid.ki * s_motor_err_0._3 + s_motor_pid.kd * (s_motor_err_0._3 - (s_motor_err_1._3 << 1) + s_motor_err_2._3);
            s_motor_out._4 += s_motor_pid.kp * (s_motor_err_0._4 - s_motor_err_1._4) + s_motor_pid.ki * s_motor_err_0._4 + s_motor_pid.kd * (s_motor_err_0._4 - (s_motor_err_1._4 << 1) + s_motor_err_2._4);

            // 将放大的输出值按比例减小
            s_motor_out._1 >>= 8;
            s_motor_out._2 >>= 8;
            s_motor_out._3 >>= 8;
            s_motor_out._4 >>= 8;
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
                LL_GPIO_ResetOutputPin(Motor2_Con1_GPIO_Port, Motor2_Con2_Pin);
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
            Motor_SetSpeed(MOTOR_1, s_motor_out._1);
            Motor_SetSpeed(MOTOR_2, s_motor_out._2);
            Motor_SetSpeed(MOTOR_3, s_motor_out._3);
            Motor_SetSpeed(MOTOR_4, s_motor_out._4);
        }
    }

    // 循迹PID
    {
        // 获取当前值
        s_ir_current = -IR_WEIGH_1 * g_ir_val._1 - IR_WEIGH_2 * g_ir_val._2 + IR_WEIGH_3 * g_ir_val._3 + IR_WEIGH_2 * g_ir_val._4 + IR_WEIGH_1 * g_ir_val._5;

        // 计算误差值
        s_ir_err_2 = s_ir_err_1;
        s_ir_err_1 = s_ir_err_0;
        s_ir_err_0 = IR_TGT - s_ir_current;

        // 计算输出值
        g_ir_out += s_ir_pid.kp * (s_ir_err_0 - s_ir_err_1) + s_ir_pid.ki * s_ir_err_0 + s_ir_pid.kd * (s_ir_err_0 - (s_ir_err_1 << 1) + s_ir_err_2);

        // 将放大的输出值按比例减小
        g_ir_out >>= 8;
    }
}
