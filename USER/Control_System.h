#ifndef __CONTROL_SYSTEM_H
#define __CONTROL_SYSTEM_H
#include "sys.h"
#include "steer.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"		
#include "steer.h"
#include "key.h"
#include "timer.h"
#include "Array.h"
#include "HC08.h"
void steer_Control(float range,float speed);
float Sigmoid_reciprocal(float L, float k_reciprocal, float x0, float x);
float Sigmoid(float L, float k, float x0, float x);
void Acc_Control(float La, float ka, float Lv, float kv, float range_max, int aazw,int ADC_delta);

void HC08_Press5();
void HC08_Press6();

#endif