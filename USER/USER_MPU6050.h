#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "tpad.h"
#include "timer.h"
#include "exti.h"
#include "usmart.h"
#include "steer.h"
#include "mpu6050.h"
#include "usmart.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "HC08.h"
#include "Control_System.h"

static float g_value;
static int LCD_en;

void usart1_send_char(u8 c);
void usart1_niming_report(u8 fun,u8*data,u8 len);
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw);
int MPU_USER_Init();

int MPU6050_Report(u8 report, long* aac, float* wRA, long* aacw, short* gyro, float* angle);
//MPU6050_Report(1, aac, wRA, aac_world, gyro, angle);
int MPU6050_Print(u8 report, long* aac, long* aacw, short* gyro, float* angle, short t);
//MPU6050_Print(1, aac, aac_world, gyro, angle,t0);	
float MPU_wRA(int t, float* wRA);