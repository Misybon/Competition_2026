#pragma once

#include "main.h"

extern DMA_HandleTypeDef hdma_usart3_rx;

extern volatile bool g_line_reached;
extern volatile bool g_color_status;
extern volatile bool g_return_flag;
extern volatile uint8_t g_corner_count;
extern volatile bool g_motor_startflag;
extern volatile bool g_usart_flag;
extern uint8_t g_rx_data[BUF_SIZE];
volatile extern uint8_t g_cmd[BUF_SIZE];
extern volatile TRACK_STATUS g_status;

void STBY_Handler(void);
void TRACK_Handler(void);
void CORNER_Handler(void);
void THROW_PREPARE_Handler(void);
void THROW_WAIT_Handler(void);
void STOP_PREPARE_Handler(void);
