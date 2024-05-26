#ifndef __STEER_H
#define __STEER_H
#include "sys.h"
#include "HC08.h"

#define PWM_V PFout(1)
#define EN_V PFout(0)
#define DIR_V PFout(2)

u8 steer_Init(void);

void steer_out(void);
void steer_move(void);
int steer_speed_choice();
#endif