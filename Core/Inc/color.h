#pragma once

struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

extern struct Color g_color;

uint32_t Color_Init(void);
void Color_Start(void);
void Color_Stop(void);
void GetColor(void);
