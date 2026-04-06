// 等待完善...
#pragma once

// PWM频率：2KHz

#include "main.h"
#include "tim.h"

#define MOTOR_1 TIM_CHANNEL_1
#define MOTOR_2 TIM_CHANNEL_2
#define MOTOR_3 TIM_CHANNEL_3
#define MOTOR_4 TIM_CHANNEL_4

#define MOTOR_ENCODER_1 TIM1
#define MOTOR_ENCODER_2 TIM2
#define MOTOR_ENCODER_3 TIM3
#define MOTOR_ENCODER_4 TIM4

struct Motor_Speed
{
    int32_t _1;
    int32_t _2;
    int32_t _3;
    int32_t _4;
};

struct Motor_TargetSpeed
{
    int32_t _1;
    int32_t _2;
    int32_t _3;
    int32_t _4;
};

extern struct Motor_Speed g_motor_speed;
extern struct Motor_TargetSpeed g_motor_tgtspeed;

/**
 * @brief 开启电机1
 */
__STATIC_INLINE void Motor1_Start(void)
{
    HAL_TIM_PWM_Start(&htim5, MOTOR_1);
    LL_GPIO_SetOutputPin(Motor1_Con1_GPIO_Port, Motor1_Con1_Pin);
}

/**
 * @brief 开启电机2
 */
__STATIC_INLINE void Motor2_Start(void)
{
    HAL_TIM_PWM_Start(&htim5, MOTOR_2);
    LL_GPIO_SetOutputPin(Motor2_Con1_GPIO_Port, Motor2_Con1_Pin);
}

/**
 * @brief 开启电机3
 */
__STATIC_INLINE void Motor3_Start(void)
{
    HAL_TIM_PWM_Start(&htim5, MOTOR_3);
    LL_GPIO_SetOutputPin(Motor3_Con1_GPIO_Port, Motor3_Con1_Pin);
}

/**
 * @brief 开启电机4
 */
__STATIC_INLINE void Motor4_Start(void)
{
    HAL_TIM_PWM_Start(&htim5, MOTOR_4);
    LL_GPIO_SetOutputPin(Motor4_Con1_GPIO_Port, Motor4_Con1_Pin);
}

/**
 * @brief 关闭电机
 * 
 * @param Motor 电机编号
 */
__STATIC_INLINE void Motor_Stop(uint32_t Motor)
{
    HAL_TIM_PWM_Stop(&htim5, Motor);
}

/**
 * @brief 电机1制动
 */
__STATIC_INLINE void Motor1_Break(void)
{
    LL_GPIO_SetOutputPin(Motor1_Con1_GPIO_Port, Motor1_Con1_Pin);
    LL_GPIO_SetOutputPin(Motor1_Con2_GPIO_Port, Motor1_Con2_Pin);
}

/**
 * @brief 电机2制动
 */
__STATIC_INLINE void Motor2_Break(void)
{
    LL_GPIO_SetOutputPin(Motor2_Con1_GPIO_Port, Motor2_Con1_Pin);
    LL_GPIO_SetOutputPin(Motor2_Con2_GPIO_Port, Motor2_Con2_Pin);
}

/**
 * @brief 电机3制动
 */
__STATIC_INLINE void Motor3_Break(void)
{
    LL_GPIO_SetOutputPin(Motor3_Con1_GPIO_Port, Motor3_Con1_Pin);
    LL_GPIO_SetOutputPin(Motor3_Con2_GPIO_Port, Motor3_Con2_Pin);
}

/**
 * @brief 电机4制动
 */
__STATIC_INLINE void Motor4_Break(void)
{
    LL_GPIO_SetOutputPin(Motor4_Con1_GPIO_Port, Motor4_Con1_Pin);
    LL_GPIO_SetOutputPin(Motor4_Con2_GPIO_Port, Motor4_Con2_Pin);
}

/**
 * @brief 重启电机1
 */
__STATIC_INLINE void Motor1_Restart(void)
{
    LL_GPIO_ResetOutputPin(Motor1_Con1_GPIO_Port, Motor1_Con1_Pin);
}

/**
 * @brief 重启电机2
 */
__STATIC_INLINE void Motor2_Restart(void)
{
    LL_GPIO_ResetOutputPin(Motor2_Con1_GPIO_Port, Motor2_Con1_Pin);
}

/**
 * @brief 重启电机3
 */
__STATIC_INLINE void Motor3_Restart(void)
{
    LL_GPIO_ResetOutputPin(Motor3_Con1_GPIO_Port, Motor3_Con1_Pin);
}

/**
 * @brief 重启电机4
 */
__STATIC_INLINE void Motor4_Restart(void)
{
    LL_GPIO_ResetOutputPin(Motor4_Con1_GPIO_Port, Motor4_Con1_Pin);
}

/**
 * @brief 设置电机速度
 * 
 * @param Motor 电机编号
 * @param Speed 电机速度，范围0~999
 */
__STATIC_INLINE void Motor_SetSpeed(uint32_t Motor, uint32_t Speed)
{
    __HAL_TIM_SET_COMPARE(&htim5, Motor, Speed);
}

/**
 * @brief 获取电机速度
 * 
 * @param Motor_Encoder 电机编码器编号
 * @return int32_t 编码器数值
 */
__STATIC_INLINE int32_t Motor_GetSpeed(TIM_TypeDef* Motor_Encoder)
{
    int32_t speed = LL_TIM_GetCounter(Motor_Encoder);
    LL_TIM_SetCounter(Motor_Encoder, 0);
    return speed;
}

// 待完善...
// 应该在函数内加入IN引脚控制
__STATIC_INLINE void Motor_Rot(uint32_t Angle)
{
}
