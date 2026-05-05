#pragma once

// 麦克纳姆轮底盘相关
#define CAR_H (0.12345F) // 底盘轮距，单位m
#define CAR_W (0.192F) // 底盘轴距，单位m
#define ROT_PARAM ((CAR_H / 2) + (CAR_W / 2)) // 旋转参数
#define MAX_VZ (2.28) // 底盘最大角速度，单位rad/s
#define MAX_VX (0.36) // 底盘最大直线速度，单位m/s

// 缓冲区长度
#define BUF_SIZE (30)

// 电机PID参数(放大256倍)
#define MOTOR_PID_KP (450)
#define MOTOR_PID_KI (130)
#define MOTOR_PID_KD (120)

// 红外PID参数(放大256倍)
#define IR_PID_KP (256)
#define IR_PID_KI (64)
#define IR_PID_KD (32)

// 电机线速度和PWM占空比对应比例
#define MOTOR_KP (2775)
// 电机编码器读取值和PWM占空比对应比例
#define MOTOR_ENCODER_KP (6)

// 红外输出值和底盘角速度对应比例
#define IR_KP (1)

// 红外目标值和各路权重
#define IR_TGT (0)
#define IR_WEIGH_1 (410)
#define IR_WEIGH_2 (178)
#define IR_WEIGH_3 (0)

// 丢线时间，单位ms
#define LINELOST_TIME (100)

// 视觉偏移值和底盘角速度比例
#define OFFSET_KP (1)

// 颜色传感器
// 增益设置
// #define GAIN_1X
#define GAIN_4X
// #define GAIN_16X
// #define GAIN_60X

// 积分时间设置
// #define ATIME_2_4MS
// #define ATIME_24MS
// #define ATIME_50MS
// #define ATIME_101MS
#define ATIME_154MS
// #define ATIME_614MS

// 投掷区颜色阈值
#define THROW_COLOR_R (0)
#define THROW_COLOR_G (0)
#define THROW_COLOR_B (0)

// 开始区颜色阈值
#define START_COLOR_R (0)
#define START_COLOR_G (0)
#define START_COLOR_B (0)

// RGB值最大偏移量
#define COLOR_MAX_OFFSET (5)
