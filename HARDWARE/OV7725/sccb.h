#ifndef __SCCB_H
#define __SCCB_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序参考自网友guanfu_wang代码。
//ALIENTEK战舰STM32开发板V3
//SCCB 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/18
//版本：V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////
  
#define SCCB_SDA_IN()  {GPIOF->CRH&=0XFF0FFFFF;GPIOF->CRH|=0X00800000;}
#define SCCB_SDA_OUT() {GPIOF->CRH&=0XFF0FFFFF;GPIOF->CRH|=0X00300000;}

//IO操作函数	 
#define SCCB_SCL    		PFout(3)	 	//SCL
#define SCCB_SDA    		PFout(13) 		//SDA	 

#define SCCB_READ_SDA    	PFin(13)  		//输入SDA    
#define SCCB_ID   			 0x24  //0X42  			//OV7670的ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u16 reg,u8 data); //改8 16
u8 SCCB_RD_Reg(u16 reg);//改了
#endif













