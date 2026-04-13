// 等待完善...

#include "task.h"
#include "main.h"

// 加校验和位
const uint8_t TASK_READY[5] = { 0xAA, 0x55, 0x01, 0x01, 0x02 };
const uint8_t TASK_RETURN[10] = { 0xAA, 0x55, 0x01, 0x02, 0x03 };
const uint8_t TASK_ACK[5] = { 0xAA, 0x55, 0x01, 'A', 0x42 };

/**
 * @brief 等待视觉应答，1000毫秒超时，重试10次
 */
void WaitForAck(void)
{
    uint32_t tick_start = HAL_GetTick();
    uint32_t retry = 0; // 重试次数

    while (!(g_cmd[0] == 'A' && g_cmd[1] == 'C' && g_cmd[2] == 'K')) // 等待收到应答命令
    {
        if (HAL_GetTick() - tick_start >= 500)
        {
            tick_start = HAL_GetTick();
            if (retry >= 10) // 重试次数过多，认定视觉掉线
            {
                g_vision_errorflag = 1; // 设定标志位
                Error_Handler(); // 进入错误处理，尝试复位
                return;
            }
            SendReady(); // 重发准备信号
            retry++; // 重试计数+1
        }
    }
}

// 待完善
void FindBasket(void)
{
}

// 待完善
void Throw(void)
{
    // 投掷代码

    // 向视觉发送返回指令
    SendReturn();
}
