#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include <stm32f10x.h>  

#include "string.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);



#define USARTx_DEBUG         USART1    // 用于重定向printf, 使printf通过USARTx发送数据





/*****************************************************************************
 ** 全局变量 
****************************************************************************/
typedef struct 
{
    uint8_t  USART1InitOK;             // 初始化标记; 0=未初始化, 1=已初始化
    uint8_t  USART1RecivedFlag;        // 接收到新的一帧数据；0-无，1-有新数据；在处理数据后，标记为0
    uint8_t  USART1RecivedCNT;         // 接收到多少个字节数据
    uint8_t  USART1RecivedBuffer[256]; // 接收到数据的缓存
    
    uint8_t  USART2InitOK;             // 初始化标记; 0=未初始化, 1=已初始化
    uint8_t  USART2RecivedFlag;        // 接收到新的一帧数据；0-无，1-有新数据；在处理数据后，标记为0
    uint8_t  USART2RecivedCNT;         // 接收到多少个字节数据
    uint8_t  USART2RecivedBuffer[256]; // 接收到数据的缓存
    
    uint8_t  USART3InitOK;             // 初始化标记; 0=未初始化, 1=已初始化
    uint8_t  USART3RecivedFlag;        // 接收到新的一帧数据；0-无，1-有新数据；在处理数据后，标记为0
    uint8_t  USART3RecivedCNT;         // 接收到多少个字节数据
    uint8_t  USART3RecivedBuffer[256]; // 接收到数据的缓存 
    
    uint8_t  UART4InitOK;              // 初始化标记; 0=未初始化, 1=已初始化
    uint8_t  UART4RecivedFlag;         // 接收到新的一帧数据；0-无，1-有新数据；在处理数据后，标记为0
    uint8_t  UART4RecivedCNT;          // 接收到多少个字节数据
    uint8_t  UART4RecivedBuffer[256];  // 接收到数据的缓存
    
    uint8_t  UART5InitOK;              // 初始化标记; 0=未初始化, 1=已初始化
    uint8_t  UART5RecivedFlag;         // 接收到新的一帧数据；0-无，1-有新数据；在处理数据后，标记为0
    uint8_t  UART5RecivedCNT;          // 接收到多少个字节数据
    uint8_t  UART5RecivedBuffer[256];  // 接收到数据的缓存
    
    uint8_t  testCNT;                  // 仅用于测试
    
}xUSATR_TypeDef;

extern xUSATR_TypeDef  xUSART;         // 声明为全局变量,方便记录信息、状态
// USART1
void  USART1_Init (uint32_t baudrate);                          // 初始化串口的GPIO、通信参数配置、中断优先级; (波特率可设、8位数据、无校验、1个停止位)
void  USART1_GetBuffer (uint8_t* buffer, uint8_t* cnt);         // 获取接收到的数据
void  USART1_SendData (uint8_t* buf, uint8_t cnt);              // 通过中断发送数据，适合各种数据
void  USART1_printf (char* stringTemp);                         // 通过中断发送字符串，适合字符串，长度在256个长度内的
void  USART1_printfForDMA(char* stringTemp) ;                   // 通过DMA发送数据，适合一次过发送数据量特别大的字符串，省了占用中断的时间
// USART2
void  USART2_Init (uint32_t baudrate);                          // 初始化串口的GPIO、通信参数配置、中断优先级; (波特率可设、8位数据、无校验、1个停止位)
void  USART2_GetBuffer (uint8_t* buffer, uint8_t* cnt);         // 获取接收到的数据
void  USART2_SendData (uint8_t* buf, uint8_t cnt);              // 通过中断发送数据，适合各种数据
void  USART2_printf (char* stringTemp);                         // 通过中断发送字符串，适合字符串，长度在256个长度内的
// USART3
void  USART3_Init (uint32_t baudrate);                          // 初始化串口的GPIO、通信参数配置、中断优先级; (波特率可设、8位数据、无校验、1个停止位)
void  USART3_GetBuffer (uint8_t* buffer, uint8_t* cnt);         // 获取接收到的数据
void  USART3_SendData (uint8_t* buf, uint8_t cnt);              // 通过中断发送数据，适合各种数据
void  USART3_printf (char* stringTemp);                         // 通过中断发送字符串，适合字符串，长度在256个长度内的
// USART4
void  UART4_Init (uint32_t baudrate);                           // 初始化串口的GPIO、通信参数配置、中断优先级; (波特率可设、8位数据、无校验、1个停止位)
void  UART4_GetBuffer (uint8_t* buffer, uint8_t* cnt);          // 获取接收到的数据
void  UART4_SendData (uint8_t* buf, uint8_t cnt);               // 通过中断发送数据，适合各种数据
void  UART4_printf (char* stringTemp);                          // 通过中断发送字符串，适合字符串，长度在256个长度内的
// USART5
void  UART5_Init (uint32_t baudrate);                           // 初始化串口的GPIO、通信参数配置、中断优先级; (波特率可设、8位数据、无校验、1个停止位)
void  UART5_GetBuffer (uint8_t* buffer, uint8_t* cnt);          // 获取接收到的数据
void  UART5_SendData (uint8_t* buf, uint8_t cnt);               // 通过中断发送数据，适合各种数据
void  UART5_printf (char* stringTemp);                          // 通过中断发送字符串，适合字符串，长度在256个长度内的

#endif


