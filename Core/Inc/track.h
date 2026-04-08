// 等待完善...

#pragma once

#include "main.h"

struct Track_Speed
{
    int32_t vx;
    int32_t vy;
    int32_t vz;
};

extern struct Track_Speed g_track_speed;

void Track_Start(void);
void Track_Stop(void);
void Track_Break(void);
void Track_Restart(void);
void ProcessLineLostEvent(void);
