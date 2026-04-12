#pragma once

// 麦克纳姆轮底盘相关
#define CAR_H (12356) // 底盘轮距，单位m，放大100000倍
#define CAR_W (19200) // 底盘轴距，单位m,放大100000倍
#define ROT_PARAM (((CAR_H * 2) + (CAR_W * 2))) // 旋转参数,放大100000倍

// 缓冲区长度
#define BUF_SIZE (30)

// 制动完成判断阈值
#define BREAK_CPLT (0)

// 制动完成判断计数和超时判断计数
#define BREAK_CPLT_CNT (5)
#define BREAK_MAX_CNT (20)

// 电机PID参数(放大256倍)
#define MOTOR_PID_KP (0)
#define MOTOR_PID_KI (0)
#define MOTOR_PID_KD (0)

// 红外PID参数(放大256倍)
#define IR_PID_KP (0)
#define IR_PID_KI (0)
#define IR_PID_KD (0)

// 电机线速度和编码器读取速度对应比例
#define MOTOR_KP (0)

// 红外输出值和底盘角速度对应比例
#define IR_KP (0)

// 红外目标值和各路权重
#define IR_TGT (0)
#define IR_WEIGH_1 (200)
#define IR_WEIGH_2 (100)
#define IR_WEIGH_3 (0)

// 颜色传感器
#define GAIN (0x02) // 增益值
#define ATIME (0xD6) // 采样时间
