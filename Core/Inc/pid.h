// 等待完善...

#pragma once
#include "main.h"

void Motor_PID_Control(void);
void IR_PID_Control(void);
void PID_Init(void);
void PID_SetControlDivider(uint32_t divider);
uint32_t PID_GetDivRatioByTargetSpeed(void);
