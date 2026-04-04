// 等待完善...

#ifndef __TRACK_H
#define __TRACK_H

#include "motor.h"

void Track_Start(void);
void Track_Stop(void);
void Track_Break(void);
void Track_Restart(void);
void ProcessLineLostEvent(void);

#endif
