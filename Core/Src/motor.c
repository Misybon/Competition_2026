// 等待完善...

#include "main.h"
#include "types.h"

struct Motor_TargetSpeed
{
    int32_t _1;
    int32_t _2;
    int32_t _3;
    int32_t _4;
} g_motor_tgtspeed = { 0 };

struct Motor_Speed
{
    int32_t _1;
    int32_t _2;
    int32_t _3;
    int32_t _4;
} g_motor_speed = { 0 };

struct Track_Speed g_track_speed = { 0 };
