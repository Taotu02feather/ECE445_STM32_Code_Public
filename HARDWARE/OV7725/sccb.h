#ifndef __SCCB_H
#define __SCCB_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ο�������guanfu_wang���롣
//ALIENTEKս��STM32������V3
//SCCB ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/18
//�汾��V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////
  
#define SCCB_SDA_IN()  {GPIOF->CRH&=0XFF0FFFFF;GPIOF->CRH|=0X00800000;}
#define SCCB_SDA_OUT() {GPIOF->CRH&=0XFF0FFFFF;GPIOF->CRH|=0X00300000;}

//IO��������	 
#define SCCB_SCL    		PFout(3)	 	//SCL
#define SCCB_SDA    		PFout(13) 		//SDA	 

#define SCCB_READ_SDA    	PFin(13)  		//����SDA    
#define SCCB_ID   			 0x24  //0X42  			//OV7670��ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u16 reg,u8 data); //��8 16
u8 SCCB_RD_Reg(u16 reg);//����
#endif













