#pragma once

#include "config.h"
#include "main.h"

#ifdef GAIN_1X
#define GAIN (0x00)
#elifdef GAIN_4X
#define GAIN (0x01)
#elifdef GAIN_16X
#define GAIN (0x02)
#elifdef GAIN_60X
#define GAIN (0x03)
#endif

#ifdef ATIME_2_4MS
#define ATIME (0xFF)
#elifdef ATIME_24MS
#define ATIME (0xF6)
#elifdef ATIME_50MS
#define ATIME (0xE8)
#elifdef ATIME_101MS
#define ATIME (0xD5)
#elifdef ATIME_154MS
#define ATIME (0xC0)
#elifdef ATIME_614MS
#define ATIME (0x00)
#endif

struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

extern struct Color g_color;
extern const struct Color START_COLOR;
extern const struct Color THROW_COLOR;

uint32_t Color_Init(void);
void Color_Start(void);
void Color_Stop(void);
void GetColor(void);
