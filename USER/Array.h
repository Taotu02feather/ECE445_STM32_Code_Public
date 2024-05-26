#ifndef __ARRAY_H 
#define __ARRAY_H
#include "sys.h"
#include <math.h>
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
//#include "usart_f103.h"
#include "USER_MPU6050.h"
#include "USER_ADC.h" 
#include "Array.h"
#include "usart.h"	 
#include "string.h"
#include "ov7725.h"
#include "tpad.h"
#include "timer.h"
#include "exti.h"
#include "usmart.h"
#include "steer.h"
#include "HC08.h"
#include "core_cm3.h"

int GetIndex(int x,int y);
void Matrix_33_33(float* AB, float* A, float* B);
void Matrix_33_31(float* Av, float* A, float* v);
double GetNorm(float* v);
double GetNormxyz(float x, float y, float z);
void GetRx(float* Rx, float roll);
void GetRy(float* Ry, float pitch);
void GetRz(float* Rz, float yaw);



void GetwRA(float* wRA, float* Aa);
void GetARB(float* ARB, float* angle);

void delay_n1000ms(int n);
int PWM_Duty_Set(int f,float Duty);

#endif
