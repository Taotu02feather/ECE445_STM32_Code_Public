#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//?????????,??????,??????????
//ALIENTEK??STM32???
//??????	   
//????@ALIENTEK
//????:www.openedv.com
//????:2012/9/3
//??:V1.0
//????,?????
//Copyright(C) ????????????? 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//???????
void KEY_Init(void) //IO???
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//??PORTA,PORTE??

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_3;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //???????
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//???GPIOE4,3

	//??? WK_UP-->GPIOA.0	  ????
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0?????,????	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA.0

}
//??????
//?????
//mode:0,??????;1,?????;
//0,????????
//1,KEY0??
//2,KEY1??
//3,KEY3?? WK_UP
//???????????,KEY0>KEY1>KEY_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//???????
	if(mode)key_up=1;  //????		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);//??? 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	    
 	return 0;// ?????
}

void Remote_KEY_Init(void) //IO???
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG,ENABLE);//??PORTE??

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;//up and down
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //???????
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//???GPIOE1, 2
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9|GPIO_Pin_10;//forward and back
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //???????
 	GPIO_Init(GPIOG, &GPIO_InitStructure);//???GPIOE1, 2

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//switch in
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //???????
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//???GPIOE1, 2
}
//mode:0,??????;1,?????;
u8 Remote_KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//???????
	if(mode)key_up=1;  //????		  
	if(key_up&&(KEY5==1||KEY6==1||KEY_IN==1||KEY7==1||KEY8==1))
	{
		delay_ms(10);//??? 
		key_up=0;
		//if(KEY5==1)return KEY5_PRES;
		if(KEY_IN==1) return KEY_IN_PRES;
		else if(KEY6==1)return KEY6_PRES;
		//else if(KEY_IN==1) return KEY_IN_PRES;
		else if(KEY5==1)return KEY5_PRES;
		else if(KEY7==1)return KEY7_PRES;
		else if(KEY8==1)return KEY8_PRES;
	}else if(KEY5==0&&KEY6==0&&KEY_IN==0&&KEY7==0&&KEY8==0)key_up=1; 	    
 	return 0;// ?????
}

void KEY_In_Init(void) //IO???
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//??PORTE??

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//up and down
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //???????
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//???GPIOE1, 2


}