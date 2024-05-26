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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define  OV7725_WINDOW_WIDTH		320 	// <=320
#define  OV7725_WINDOW_HEIGHT		240 	// <=240


const u8*LMODE_TBL[6]={"Auto","Sunny","Cloudy","Office","Home","Night"};//6种光照模式	    
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7种特效 
extern u8 ov_sta;	//在exit.c里 面定义
extern u8 ov_frame;	//在timer.c里面定义	
extern float PWM_Ini_0; // Define in steer.c
extern float PWM_Press_7; // Define in steer.c
extern float PWM_Press_8; // Define in steer.c
extern int PWM_Flag_1; // Define in steer.c
extern int steer_speed_Flag; // Define in steer.c

 int main(void)
 {
	long aac[3];
	long aac_world[3];
	long aac_average[3];
	long aac_world_average[3];
	//short aacx0, aacy0, aacz0; 已经变为全局变量
	short gyro[3];
	float angle[3];
	int ADC_value_before;
	int ADC_value;
	int ADC_delta;
	//float g_value;
	float p1;
	int aacwz_HC08;
	int aacwz_Control;
	int Sensor_Flag;
	int Control_Flag;
	//char aacwz_HC08_str[15];
	 
	char aacwz_HC08_str[15];
	char L_k_va_str[20];
	char *AAC_word = "AAC 100000";
	
	size_t len_AACwz;
	char print_test[12];
	//char ADC_HC08_str[10];
	float wRA[9];
	//float ARB[9];
	short t0_aac;
	float Duty_PMW;
	
	float La,ka,Lv,kv,range_max;
	float La0,ka0,Lv0,kv0,range_max0;
	float adj_range;
	t0_aac = 0; //用来控制print频率
	// int LCD_en;
	 LCD_en = 0;
	 steer_speed_Flag = 0;
	// get the AAC string space
	// len_AACwz = strlen(AAC_word);
	// aacwz_HC08_str = (char *)malloc((len_AACwz + 1) * sizeof(char));
	// memset(aacwz_HC08_str, 0, len_AACwz*sizeof(char));
	/*
	
	*/
	La0 = 400.0;
	La = La0;
	ka0 = 1.0/1000.0;
	ka = ka0;
	Lv0 = 800.0;
	Lv = Lv0;
	kv0 = 1.0/2.0;
	kv = kv0;
	range_max0 = 800.0;
	range_max = range_max0;
	adj_range = 0.05; // 5 percent
	
	Remote_KEY_Init();
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	USART1_Init(115200);
	//uart_init(115200);	 	//串口初始化为 115200
 	usmart_dev.init(72);		//初始化USMART		
	
	if( LCD_en  == 1){
		LED_Init();		  			//初始化与LED连接的硬件接口
		LCD_Init();			   		//初始化LCD  
		
	}
 
	KEY_Init();					//初始化按键
	TPAD_Init(6);				//触摸按键初始化 
 	HC08_Init(); 
	HC08_SendString("HC08_Init \n");
	xUSART.USART1RecivedFlag=0; 
	HC08_SendString("Sensor_Open \n");
	xUSART.USART1RecivedFlag=0; 
	while(1){
			if(xUSART.USART2RecivedFlag==1){
				HC08_SendString((char*)xUSART.USART2RecivedBuffer);
				xUSART.USART2RecivedFlag=0;      
				if(xUSART.USART2RecivedBuffer[0]=='y'){
					Sensor_Flag = 1;
					HC08_SendString(" Sensor Open Y\n");
					xUSART.USART1RecivedFlag=0;
					break;

				}
				if(xUSART.USART2RecivedBuffer[0]=='n'){
					Sensor_Flag = 0;
					HC08_SendString(" Sensor Open N\n");
					xUSART.USART1RecivedFlag=0;
					break;
			}
		} 
	}
	
	steer_Init();
	KEY_In_Init();
	
	
	//EXTI2_Init();

// TIM3_Int_Init(499,7199);//10Khz的计数频率，计数到5000为500ms 
	if(Sensor_Flag==1){
		
		MPU_USER_Init();					//初始化MPU6050 
		Adc_Init();		  		//ADC初始化	
		g_value = MPU_wRA(100, wRA); 
		aac_average[0] = 0;
		aac_average[1] = 0;
		aac_average[2] = 0;
		aac_world_average[0] = 0;
		aac_world_average[1] = 0;
		aac_world_average[2] = 0;
		
		//ADC begin
		ADC_value_before = ADC_Get();
		ADC_value = ADC_Get();
		ADC_delta = ADC_value - ADC_value_before;
		
	//ADC end
		HC08_SendString("Sensor Ini \n");
		xUSART.USART1RecivedFlag=0; 
	
	}
	
	/// PWM begin //////////
	PWM_Ini_0 = 6.7; //6.7
	PWM_Press_7 = 8 ; // 8
	PWM_Press_8 = 6.3; //6.3
	PWM_Flag_1 = 1;
	//delay_n1000ms(5);
	TIM3_PWM_Init(200-1,5000-1);	 //不分频。PWM频率=72000000/900=80Khz
	TIM_SetCompare3(TIM3,PWM_Duty_Set(200,PWM_Ini_0  ));
	/// PWM end //////////
	Control_Flag = 0;
	
	//HC08_SendString("Flag01 \n");
	//xUSART.USART1RecivedFlag=0; 
	
		HC08_SendString("Ini Complete\n");
		xUSART.USART1RecivedFlag=0; 
	while(1){
		//memset(print_test, 0, sizeof(print_test));
		//sprintf(print_test,"TEST %d\n ",(int)(100*exp(1.0))); 
		//HC08_SendString(print_test);
		//xUSART.USART1RecivedFlag=0; 
		if(xUSART.USART2RecivedFlag==1){
			HC08_SendString((char*)xUSART.USART2RecivedBuffer);
      xUSART.USART2RecivedFlag=0;      
			if(xUSART.USART2RecivedBuffer[0]=='r'){
				HC08_SendString("reset\n");
				xUSART.USART1RecivedFlag=0;
				__set_FAULTMASK(1);
				NVIC_SystemReset();
			}
			if(xUSART.USART2RecivedBuffer[0]=='a'){
				HC08_SendString("Control All Open\n");
				xUSART.USART1RecivedFlag=0;
				Control_Flag = 2;
			}
			if(xUSART.USART2RecivedBuffer[0]=='o'){
				HC08_SendString("Control Only AAC Open\n");
				xUSART.USART1RecivedFlag=0;
				Control_Flag = 1;
			}
			if(xUSART.USART2RecivedBuffer[0]=='c'){
				HC08_SendString("Control Close\n");
				xUSART.USART1RecivedFlag=0;
				Control_Flag = 0;
			}
			//steer_speed_Flag = 0;
			if(xUSART.USART2RecivedBuffer[0]=='+'){
				if( steer_speed_Flag == 2 ){
					HC08_SendString("Speed Reset\n");
					xUSART.USART1RecivedFlag=0;
					steer_speed_Flag = 0;
				}
				else{
					HC08_SendString("Speed Added\n");
					xUSART.USART1RecivedFlag=0;
					steer_speed_Flag = steer_speed_Flag + 1;
				}
			}
			// adj_range
			if(xUSART.USART2RecivedBuffer[0]=='1'){
				HC08_SendString(" adj_range is now 1percent\n");
				xUSART.USART1RecivedFlag=0;
				adj_range = 0.01;
			}
			if(xUSART.USART2RecivedBuffer[0]=='2'){
				HC08_SendString(" adj_range is now 5 percent\n");
				xUSART.USART1RecivedFlag=0;
				adj_range = 0.05;
			}
			if(xUSART.USART2RecivedBuffer[0]=='3'){
				HC08_SendString(" adj_range is now 10 percent\n");
				xUSART.USART1RecivedFlag=0;
				adj_range = 0.10;
			}
			
			
			// La w s
			if(xUSART.USART2RecivedBuffer[0]=='w'){
				HC08_SendString(" La + \n");
				xUSART.USART1RecivedFlag=0;
				La = La0*adj_range + La;
				if( La >= La0 * 3.0){
					La = La0*3.0;
				}
			}
			if(xUSART.USART2RecivedBuffer[0]=='s'){
				HC08_SendString(" La - \n");
				xUSART.USART1RecivedFlag=0;
				La = -La0*adj_range + La;
				if( La <= La0/4.0){
					La = La0/4.0;
				}
			}
			
			// ka e d
			if(xUSART.USART2RecivedBuffer[0]=='e'){
				HC08_SendString(" ka + \n");
				xUSART.USART1RecivedFlag=0;
				ka = ka0*adj_range + ka;
				if( ka >= ka0 * 3.0){
					ka = ka0*3.0;
				}
			}
			if(xUSART.USART2RecivedBuffer[0]=='d'){
				HC08_SendString(" ka - \n");
				xUSART.USART1RecivedFlag=0;
				ka = -ka0*adj_range + ka;
				if( ka <= ka0/4.0){
					ka = ka0/4.0;
				}
			}
			
			// Lv u j
			if(xUSART.USART2RecivedBuffer[0]=='u'){
				HC08_SendString(" Lv + \n");
				xUSART.USART1RecivedFlag=0;
				Lv = Lv0*adj_range + Lv;
				if( Lv >= Lv0 * 3.0){
					Lv = Lv0*3.0;
				}
			}
			if(xUSART.USART2RecivedBuffer[0]=='j'){
				HC08_SendString(" Lv - \n");
				xUSART.USART1RecivedFlag=0;
				Lv = -Lv0*adj_range + Lv;
				if( Lv <= Lv0/4.0){
					Lv = Lv0/4.0;
				}
			}
			
				// kv i k
			if(xUSART.USART2RecivedBuffer[0]=='i'){
				HC08_SendString(" kv + \n");
				xUSART.USART1RecivedFlag=0;
				kv = kv0*adj_range + kv;
				if( kv >= kv0 * 3.0){
					kv = kv0*3.0;
				}
			}
			if(xUSART.USART2RecivedBuffer[0]=='k'){
				HC08_SendString(" kv - \n");
				xUSART.USART1RecivedFlag=0;
				kv = -kv0*adj_range + kv;
				if( kv <= kv0/4.0){
					kv = kv0/4.0;
				}
			}
			
			// range
			if(xUSART.USART2RecivedBuffer[0]=='z'){
				HC08_SendString(" range + \n");
				xUSART.USART1RecivedFlag=0;
				range_max = range_max0*adj_range + range_max;
				if( range_max >= range_max0 * 3.0){
					range_max = range_max0*3.0;
				}
			}
			if(xUSART.USART2RecivedBuffer[0]=='x'){
				HC08_SendString(" range - \n");
				xUSART.USART1RecivedFlag=0;
				range_max = -range_max0*adj_range + range_max;
				if( range_max <= range_max0/4.0){
					range_max = range_max0/4.0;
				}
			}
			// print L_k_va_str
			// float La,ka,Lv,kv,range_max;
			if(xUSART.USART2RecivedBuffer[0]=='p'){
				sprintf(L_k_va_str,"La: %4.6f \n ",La);
				HC08_SendString(L_k_va_str);
				xUSART.USART1RecivedFlag=0;
				
				sprintf(L_k_va_str,"ka: %4.6f \n ",ka);
				HC08_SendString(L_k_va_str);
				xUSART.USART1RecivedFlag=0;
				
				sprintf(L_k_va_str,"Lv: %4.6f \n ",Lv);
				HC08_SendString(L_k_va_str);
				xUSART.USART1RecivedFlag=0;
				
				sprintf(L_k_va_str,"kv: %4.6f \n ",kv);
				HC08_SendString(L_k_va_str);
				xUSART.USART1RecivedFlag=0;
				
				sprintf(L_k_va_str,"range: %4.6f \n ",range_max);
				HC08_SendString(L_k_va_str);
				xUSART.USART1RecivedFlag=0;
				
			}
			
			if(xUSART.USART2RecivedBuffer[1]=='5'){
					HC08_SendString("\nPress 5 Begin\n");
					xUSART.USART1RecivedFlag=0;
					HC08_Press5();	
			}
			if(xUSART.USART2RecivedBuffer[1]=='6'){
					HC08_SendString("\nPress 6 Begin\n");
					xUSART.USART1RecivedFlag=0;
					HC08_Press6();
			}
		}   
		
		steer_move();
		
		//continue;
		
		if(Sensor_Flag==0){
			continue;
		}
		
		if(MPU6050_Report(1, aac, wRA, aac_world, gyro, angle) != 0){
			
			continue;
		}
		
		
		
		t0_aac++;
		
			 
		
		aac_average[0] = aac_average[0] + aac[0];
		aac_average[1] = aac_average[1] + aac[1];
		aac_average[2] = aac_average[2] + aac[2];
		aac_world_average[0] = aac_world_average[0] + aac_world[0];
		aac_world_average[1] = aac_world_average[1] + aac_world[1];
		aac_world_average[2] = aac_world_average[2] + aac_world[2];
		
		
		if(t0_aac%5 == 0){
			
			
			aac_average[0] = aac_average[0]/t0_aac;
			aac_average[1] = aac_average[1]/t0_aac;
			aac_average[2] = aac_average[2]/t0_aac;
			aac_world_average[0] = aac_world_average[0]/t0_aac;
			aac_world_average[1] = aac_world_average[1]/t0_aac;
			aac_world_average[2] = aac_world_average[2]/t0_aac;
			
			//HC08_SendString("Flag0003 \n");
			//xUSART.USART1RecivedFlag=0;
			// (aacw[2] - (short)g_value)
			aacwz_HC08 = MPU6050_Print(1, aac_average, aac_world_average, gyro, angle,t0_aac);	
			
			//HC08_SendString("Flag00004 \n");
			//xUSART.USART1RecivedFlag=0;
			ADC_value_before = ADC_value;
			ADC_value = ADC_Get();
			ADC_delta = ADC_value - ADC_value_before;
			
			if( LCD_en == 0 ){
				//HC08_SendString("Flag002 \n");
				//xUSART.USART1RecivedFlag=0;
				// memset(aacwz_HC08_str, 0, sizeof(char)*strlen(aacwz_HC08_str));
				// sprintf(ADC_HC08_str,"ADC %d \n",num);
				sprintf(aacwz_HC08_str,"AAC %d \n ",aacwz_HC08);
				// print the aac of world of z to HC08
				HC08_SendString(aacwz_HC08_str);
				xUSART.USART1RecivedFlag=0; 
				//ADC_HC08_str
				//HC08_SendString("Flag003 \n");
				//xUSART.USART1RecivedFlag=0;
				ADC_Print( ADC_value );
				
				
				
			}
			
			aacwz_Control = aac_world_average[2]-(short)g_value;
			if(Control_Flag == 1){
				Acc_Control( La, ka, Lv, kv, range_max, aacwz_Control,0.0);
			}
			else if(Control_Flag ==2){
				Acc_Control( La, ka, Lv, kv, range_max, aacwz_Control,ADC_delta);
				//Acc_Control(aacwz_Control,ADC_delta);
			}
			//HC08_SendString("Flag000005 \n");
			//xUSART.USART1RecivedFlag=0;
			
			t0_aac = 0;
			aac_average[0] = 0;
			aac_average[1] = 0;
			aac_average[2] = 0;
			aac_world_average[0] = 0;
			aac_world_average[1] = 0;
			aac_world_average[2] = 0;
			
			
			
			
		}

		

		
	}

	


}









