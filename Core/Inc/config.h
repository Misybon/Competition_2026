#pragma once

#define CAR_H (0) // 底盘轮距
#define CAR_W (0) // 底盘轴距
#define ROT_PARAM ((CAR_H >> 1) + (CAR_W >> 1)) // 旋转参数

// 缓冲区长度
#define BUF_SIZE (30)

// 电机PID参数
#define MOTOR_PID_KP (0)
#define MOTOR_PID_KI (0)
#define MOTOR_PID_KD (0)

// 红外PID参数
#define IR_PID_KP (0)
#define IR_PID_KI (0)
#define IR_PID_KD (0)

// 红外目标值和各路权重
#define IR_TGT (0)
#define IR_WEIGH_1 (200)
#define IR_WEIGH_2 (100)
#define IR_WEIGH_3 (0)

// 颜色传感器
#define GAIN (0x02) // 增益值
#define ATIME (0xD6) // 采样时间
