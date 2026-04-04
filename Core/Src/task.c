// 等待完善...

#include "task.h"
#include "main.h"

const uint8_t TASK_READY[9] = { 0xAA, 0x55, 0x05, 'R', 'E', 'A', 'D', 'Y' };
const uint8_t TASK_RETURN[10] = { 0xAA, 0x55, 0x06, 'R', 'E', 'T', 'U', 'R', 'N' };

void Throw(void)
{
    // 投掷代码

    // 向视觉发送返回指令
    SendReturn();
}
