#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
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


//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define WK_UP PAin(0)	//PA0  WK_UP

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//????0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//????1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//????3(WK_UP) 
#define KEY6  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)
#define KEY5  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)
#define KEY7  GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)
#define KEY8  GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10)

#define KEY_IN GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)
 

#define KEY0_PRES 	1	//KEY0??
#define KEY1_PRES	  2	//KEY1??
#define WKUP_PRES   3	//KEY_UP??(?WK_UP/KEY_UP)

#define KEY5_PRES 	1	//KEY0??
#define KEY6_PRES	  2	//KEY1??
#define KEY_IN_PRES 3 
#define KEY7_PRES 	7	//KEY0??
#define KEY8_PRES	  8	//KEY1??
void KEY_Init(void);//IO???
u8 KEY_Scan(u8);  	//??????
void Remote_KEY_Init(void);
u8 Remote_KEY_Scan(u8 mode);
void KEY_In_Init(void);
#endif
