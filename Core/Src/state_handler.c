#include "main.h"

volatile bool g_motor_startflag = 0; // 电机标志位
volatile bool g_color_status = 0; // 颜色传感器标志位

volatile bool g_status_errorflag = 0; // 状态机错误标志位

volatile bool g_line_reached = 0; // 是否已经到达线上
volatile bool g_start_area_flag = 0; // 是否处于开始区
volatile bool g_throw_area_flag = 0; // 是否处于投掷区
volatile bool g_return_flag = 0; // 是否处于返回过程

volatile uint8_t g_corner_count = 0; // 转弯计数

uint8_t g_rx_data[BUF_SIZE] = { 0 }; // 串口接收缓冲区
volatile uint8_t g_cmd[BUF_SIZE] = { 0 }; // 解析出来的命令

volatile bool g_usart_flag = 0; // 是否解析出一帧命令
