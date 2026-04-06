#include "color.h"
#include "i2c.h"
#include "main.h"

#define COLOR_ADDR (0x29 << 1) // 7位地址

#define COLOR_CMD_BIT 0x80 // 命令位
#define COLOR_CMD_AUTOINC 0xA0 // 地址自增

#define COLOR_ENA 0x00
#define COLOR_ATIME 0x01 // 采样时间寄存器
#define COLOR_GAIN 0x0F // 增益值寄存器
#define COLOR_ID 0x12 // ID寄存器
#define COLOR_CDATAL 0x14

#define COLOR_PEN 0x01 // 开启电源
#define COLOR_AEN 0x02 // ADC使能

struct Origin_Color
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t clear;
};

struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} g_color = { 0 };

static struct Origin_Color s_org_color = { 0 };

/**
 * @brief 写一位寄存器
 * 
 * @param Addr 寄存器地址
 * @param Byte 要写入的数据
 */
static void Color_WriteByte(uint8_t Addr, uint8_t Byte)
{
    uint8_t buf[2];
    buf[0] = COLOR_CMD_BIT | Addr;
    buf[1] = Byte;
    HAL_I2C_Master_Transmit(&hi2c1, COLOR_ADDR, buf, 2, 100);
}

/**
 * @brief What can I say
 */
__STATIC_INLINE void Color_Sleep(void)
{
    Color_WriteByte(0x00, 0x00);
    LL_mDelay(3);
}

/**
 * @brief What can I say
 *
 * @note 如需读取数据，还要考虑采样时间
 */
__STATIC_INLINE void Color_Wakeup(void)
{
    Color_WriteByte(COLOR_ENA, COLOR_PEN | COLOR_AEN);
    LL_mDelay(3);
}

/**
 * @brief 初始化颜色传感器
 * 
 * @return uint32_t 1:成功；0:失败
 */
uint32_t Color_Init(void)
{
    uint32_t id = 0;
    uint8_t cmd = COLOR_CMD_BIT | COLOR_ID;

    // 检查设备是否存在
    HAL_I2C_Master_Transmit(&hi2c1, COLOR_ADDR, &cmd, 1, 100);
    HAL_I2C_Master_Receive(&hi2c1, COLOR_ADDR, (uint8_t*)&id, 1, 100);
    if (id != 0x4D && id != 0x44)
    {
        return 0;
    }

    // 设置积分时间
    Color_WriteByte(COLOR_ATIME, ATIME);

    // 设置增益
    Color_WriteByte(COLOR_GAIN, GAIN);

    // 开启电源
    Color_WriteByte(COLOR_ENA, COLOR_PEN);

    LL_mDelay(3);

    // 开启ADC
    Color_WriteByte(COLOR_ENA, COLOR_PEN | COLOR_AEN);

    // 睡眠
    Color_Sleep();

    // 成功
    return 1;
}

/**
 * @brief 唤醒颜色传感器，点亮LED
 *
 * @note 注意采样时间
 */
void Color_Start(void)
{
    g_color_status = 1;
    LL_GPIO_SetOutputPin(COLOR_LED_GPIO_Port, COLOR_LED_Pin);
    Color_Wakeup();
}

/**
 * @brief 休眠颜色传感器，熄灭LED
 */
void Color_Stop(void)
{
    g_color_status = 0;
    LL_GPIO_ResetOutputPin(COLOR_LED_GPIO_Port, COLOR_LED_Pin);
    Color_Sleep();
}

/**
 * @brief 获取颜色值
 *
 * @note 注意采样时间
 */
void GetColor(void)
{
    uint8_t buf[8];

    uint8_t cmd = COLOR_CMD_BIT | COLOR_CMD_AUTOINC | COLOR_CDATAL;
    HAL_I2C_Mem_Read(&hi2c1, COLOR_ADDR, cmd, I2C_MEMADD_SIZE_8BIT, buf, 8, 100);

    s_org_color.clear = (uint16_t)(buf[1] << 8) | buf[0]; // Clear
    s_org_color.red = (uint16_t)(buf[3] << 8) | buf[2]; // Red
    s_org_color.green = (uint16_t)(buf[5] << 8) | buf[4]; // Green
    s_org_color.blue = (uint16_t)(buf[7] << 8) | buf[6]; // Blue

    if (s_org_color.clear == 0)
    {
        g_color.red = 0;
        g_color.green = 0;
        g_color.blue = 0;
    }
    else
    {
        g_color.red = s_org_color.red * 255 / s_org_color.clear;
        g_color.green = s_org_color.green * 255 / s_org_color.clear;
        g_color.blue = s_org_color.blue * 255 / s_org_color.clear;
    }
}
