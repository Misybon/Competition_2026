#pragma once

#include "main.h"

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
