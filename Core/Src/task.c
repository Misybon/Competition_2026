// 等待完善...

#include "task.h"
#include "main.h"

const uint8_t TASK_READY[9] = { 0xAA, 0x55, 0x05, 'R', 'E', 'A', 'D', 'Y' };
const uint8_t TASK_RETURN[10] = { 0xAA, 0x55, 0x06, 'R', 'E', 'T', 'U', 'R', 'N' };

/**
 * @brief 等待视觉应答，1000毫秒超时，重试三次
 */
void WaitForAck(void)
{
    uint32_t tick_start = HAL_GetTick();
    uint32_t retry = 0;
    while (!(g_cmd[0] == 'A' && g_cmd[1] == 'C' && g_cmd[2] == 'K'))
    {
        if (HAL_GetTick() - tick_start >= 1000)
        {
            tick_start = HAL_GetTick();
            if (retry >= 3)
            {
                g_vision_errorflag = 1;
                Error_Handler();
                return;
            }
            SendReady();
            retry++;
        }
    }
}

// 待完善
void Throw(void)
{
    // 投掷代码

    // 向视觉发送返回指令
    SendReturn();
}
