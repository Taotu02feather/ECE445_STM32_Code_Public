#include "sys.h"
#include "delay.h"
#include "usart.h"		
#include "steer.h"
#include "key.h"
#include "timer.h"
#include "Array.h"
u16 x;
u8 press;
float Duty_PMW1;

float PWM_Ini_0; // Define in steer.c
float PWM_Press_7; // Define in steer.c
float PWM_Press_8; // Define in steer.c
int PWM_Flag_1; // Define in steer.c
int steer_speed_Flag; // Define in steer.c

int steer_speed_choice(){
	if( steer_speed_Flag == 0 ){
		return 3000;
	}
	if( steer_speed_Flag == 1){
		return 2000;
	}
	if( steer_speed_Flag== 2){
		return 1667;
	}
	return 3000;
	//  steer_speed_choice(steer_speed_Flag);
}

u8 steer_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 // 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO????50MHz
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOF,GPIO_Pin_1);						 // GPF2 DIR
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 // 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO????50MHz
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOF,GPIO_Pin_2);						 // GPF2 DIR
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 // 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO????50MHz
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOF,GPIO_Pin_0);						 // GPF2 DIR
//  GPIO_InitTypeDef  GPIO_InitStructure;
// 	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 // 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //
// 	GPIO_Init(GPIOF, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOF,GPIO_Pin_2);						 // GPF2 DIR

//  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 // 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //
// 	GPIO_Init(GPIOF, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOF,GPIO_Pin_0);						 // GPF0 EN

//  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 // 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //
// 	GPIO_Init(GPIOF, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOF,GPIO_Pin_1);						 // GPF1 PWM  

    return 0x01;
}

void steer_out(void){
		EN_V= 0;
		DIR_V=1;
  	for( x= 0; x<2000;x++)
  	{

  		PWM_V= 1;
  		delay_us(1000);
  		PWM_V= 0;
  		delay_us(1000);
			
  	}
  	//delay_us(1000);
		EN_V=1;
		DIR_V=0;
		delay_us(5000);
		EN_V=0;
  	for( x= 0; x<2000;x++)
  	{
//  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, SET);
//  		HAL_GPIO_WritePin(Step_GPIO_Port, Step_Pin, SET);

			PWM_V=0;
  		delay_us(2000);
			PWM_V=1;
//  		HAL_GPIO_WritePin(Step_GPIO_Port, Step_Pin, RESET);
  		delay_us(2000);
  	}
		EN_V = 1;
  	delay_us(1000);
}

void steer_move(void){
	EN_V= 1;
	press=Remote_KEY_Scan(1);
		if (press==3){
		delay_us(2000);
		EN_V=0;
		DIR_V=1;
		for( x= 0; x<100;x++)
		{

				PWM_V= 1;
				delay_us(steer_speed_choice(steer_speed_Flag)); //set the delay value to change motor speed
				PWM_V= 0;
				delay_us(steer_speed_choice(steer_speed_Flag));
			}
		EN_V=1;
		return	;
	}
		
		if (press==1){ //5
		delay_us(2000);
		EN_V=0;
		DIR_V=0;
		for( x= 0; x<100;x++)
		{

				PWM_V= 1;
				delay_us(steer_speed_choice()); //set the delay value to change motor speed
				PWM_V= 0;
				delay_us(steer_speed_choice());
			}
	}
	
	if (press==2){ //6
		delay_us(2000);
		EN_V=0;
		DIR_V=1;
		for( x= 0; x<100;x++)
		{

				PWM_V= 1;
				delay_us(steer_speed_choice());
				PWM_V= 0;
				delay_us(steer_speed_choice());
			}
	}	
	if (press==7){
		delay_us(2000);
		TIM_SetCompare3(TIM3,PWM_Duty_Set(200, PWM_Press_7));
		delay_ms(500);

		TIM_SetCompare3(TIM3,PWM_Duty_Set(200, PWM_Ini_0));
		return;
	}
	
	if (press==8){
		delay_us(2000);
		TIM_SetCompare3(TIM3,PWM_Duty_Set(200, PWM_Press_8));
		delay_ms(500);
		TIM_SetCompare3(TIM3,PWM_Duty_Set(200, PWM_Ini_0));
		return;
	}
	EN_V=1;

	
}