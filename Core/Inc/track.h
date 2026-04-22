#pragma once

#include "config.h"
#include "ir.h"
#include "motor.h"

struct Track_Speed
{
    int32_t vx;
    int32_t vy;
    int32_t vz;
};

extern struct Track_Speed g_track_speed;
extern volatile bool g_break_flag;

void Track_Start(void);
void Track_Stop(void);
void Track_Break(void);
// void Track_Restart(void);
void ProcessLineLostEvent(void);
void Track_Rot_Angle(int32_t Angle);

/**
 * @brief 整车软制动(滑行)，只清除速度
 */
__STATIC_INLINE void Track_Break_Soft(void)
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
