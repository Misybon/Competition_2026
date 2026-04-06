#ifndef COLOR_H
#define COLOR_H

#include "main.h"

struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

extern struct Color g_color;

/**
 * @brief 判断是否到达开始区域
 * 
 * @return 到达则返回1
 */
__STATIC_INLINE uint32_t IsStartAreaReached(void)
{
    return g_start_area_flag;
}

/**
 * @brief 判断是否到达投掷区
 * 
 * @return 到达则返回1
 */
__STATIC_INLINE uint32_t IsThrowAreaReached(void)
{
    return g_throw_area_flag;
}

uint32_t Color_Init(void);
void Color_Start(void);
void Color_Stop(void);
void GetColor(void);

#endif
