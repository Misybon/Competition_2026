#include "pid.h"
#include "ir.h"
#include "main.h"
#include "motor.h"

#define MOTOR_PID_KP 0
#define MOTOR_PID_KI 0
#define MOTOR_PID_KD 0

#define IR_PID_KP 0
#define IR_PID_KI 0
#define IR_PID_KD 0

#define MOTOR_MAX_SPEED 999

#define IR_TGT 0
#define IR_WEIGH_1 200
#define IR_WEIGH_2 100
#define IR_WEIGH_3 0

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
int32_t g_ir_out = 0;

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

void PID_Init(void)
{
    s_ir_pid.kp = IR_PID_KP;
    s_ir_pid.ki = IR_PID_KI;
    s_ir_pid.kd = IR_PID_KD;

    s_motor_pid.kp = MOTOR_PID_KP;
    s_motor_pid.ki = MOTOR_PID_KI;
    s_motor_pid.kd = MOTOR_PID_KD;
}

// 控制内容：循迹PID，电机PID
void PID_Control(void)
{
    // 电机PID
    {
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

            // 应该将放大的输出值按比例减小
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

        // 应该将放大的输出值按比例减小
    }
}
