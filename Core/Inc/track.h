#pragma once

#include <stdlib.h>
#include "color.h"
#include "config.h"
#include "ir.h"
#include "motor.h"
#include "pid.h"

struct Track_Speed
{
    int32_t vx;
    int32_t vy;
    int32_t vz;
};

extern struct Track_Speed g_track_speed;

void Track_Start(void);
void Track_Stop(void);
void ProcessLineLostEvent(void);
void Track_Rot_Angle(int32_t Angle);

/**
 * @brief 是否到达投掷区
 * 
 * @return uint32_t 到达1，否则0
 */
__STATIC_INLINE uint32_t IsThorwAreaReached(void)
{
    return (abs(g_color.red - THROW_COLOR_R) <= COLOR_MAX_OFFSET && abs(g_color.green - THROW_COLOR_G) <= COLOR_MAX_OFFSET && abs(g_color.blue - THROW_COLOR_B) <= COLOR_MAX_OFFSET);
}

/**
 * @brief 是否到达开始区
 * 
 * @return uint32_t 到达1，否则0
 */
__STATIC_INLINE uint32_t IsStartAreaReached(void)
{
    return (abs(g_color.red - START_COLOR_R) <= COLOR_MAX_OFFSET && abs(g_color.green - START_COLOR_G) <= COLOR_MAX_OFFSET && abs(g_color.blue - START_COLOR_B) <= COLOR_MAX_OFFSET);
}

/**
 * @brief 整车制动
 */
__STATIC_INLINE void Track_Break(void)
{
    g_track_speed.vx = 0;
    g_track_speed.vy = 0;
    g_track_speed.vz = 0;
}

/**
 * @brief 重启循迹
 */
__STATIC_INLINE void Track_Restart(void)
{
    g_track_speed.vx = MAX_VX; // 无须坡度启动
}
