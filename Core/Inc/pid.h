// 等待完善...

#pragma once

#include "main.h"

struct Motor_PID
{
    int32_t kp;
    int32_t ki;
    int32_t kd;
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

void PID_Control(void);
void PID_Init(void);
