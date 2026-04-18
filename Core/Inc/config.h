#pragma once

// 麦克纳姆轮底盘相关
#define CAR_H (12356) // 底盘轮距，单位m，放大100000倍
#define CAR_W (19200) // 底盘轴距，单位m,放大100000倍
#define ROT_PARAM (((CAR_H * 2) + (CAR_W * 2))) // 旋转参数,放大100000倍
#define MAX_VZ (1) // 底盘最大角速度，单位rad/s
#define MAX_VX (1) // 底盘最大直线速度，单位m/s

// 缓冲区长度
#define BUF_SIZE (30)

// 制动完成判断阈值
#define BREAK_CPLT (10)

// 制动完成判断计数和超时判断计数，总时间需乘上50ms
#define BREAK_CPLT_CNT (5)
#define BREAK_MAX_CNT (20)

// 电机PID参数
#define MOTOR_PID_KP (0.9F)
#define MOTOR_PID_KI (0.05F)
#define MOTOR_PID_KD (0.1F)

// 红外PID参数(放大256倍)
#define IR_PID_KP (0)
#define IR_PID_KI (0)
#define IR_PID_KD (0)

// 电机线速度和PWM占空比对应比例
#define MOTOR_KP (1)
// 电机编码器读取值和PWM占空比对应比例
#define MOTOR_ENCODER_KP (6.25F)

// 电机速度一阶低通滤波系数(0~1)
#define MOTOR_SPEED_LPF_ALPHA (0.2F)

// 电机输出死区阈值(PWM计数)
#define MOTOR_OUT_DEADZONE (10)

// 红外输出值和底盘角速度对应比例
#define IR_KP (1)

// 红外目标值和各路权重
#define IR_TGT (0)
#define IR_WEIGH_1 (200)
#define IR_WEIGH_2 (100)
#define IR_WEIGH_3 (0)

// 丢线时间，单位ms
#define LINELOST_TIME (100)

// 视觉偏移值和底盘角速度比例
#define OFFSET_KP (1)

// 颜色传感器
#define GAIN (0x02) // 增益值
#define ATIME (0xD6) // 采样时间
