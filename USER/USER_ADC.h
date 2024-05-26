#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "USER_MPU6050.h"
#include "HC08.h"

u16 ADC_Get();
int ADC_Print(u16 num);