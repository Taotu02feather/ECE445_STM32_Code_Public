#ifndef __USART_H
#define __USART_H
/***********************************************************************************************************************************
 **【购买链接】  魔女科技    https://demoboard.taobao.com
 **【更新分享】  Q群文件夹   887199504
 ***********************************************************************************************************************************
 **【文件名称】  usart.h
 **【文件功能】  本文件简化的USART的初始化，及完善了收、发功能函数；初始化后调用函数即可使用；
 **
 **【适用平台】  STM32F103 + 标准库v3.5 + keil5
 **
 **【使用说明】  移 植 ：复制USART文件夹下c、h文件到目标文件夹，添加到工程管理器, 添加文件路径, 最后 #include "usart.h＂；
 **              初始化: 调用 USARTx_Init(波特率), 函数内已做好引脚及时钟配置； 
 **              发 送 : 字符串-USARTx_printf (char* stringTemp)
 **                      数据  -USARTx_SendData (uint8_t* buf, uint8_t cnt);
 **              接 收 : 方式1 -外部代码判断xUSART.USARTxRecivedFlag==１,然后处理xUSART.USARTxRecivedBuffer[256]的数据；
 **                      方式2 -调用函数vUSARTx_GetBuffer(u8* buf, u8* cnt); 
 **                            
 **
 **【更新记录】  2020-09-02  文件建立、USART1接收中断、空闲中断、发送中断、DMA收发
 **              2021-06-04  USART1、2、3及UART4、5 的收发完善代码
 **              2021-06-09  完善文件格式、注释  
 **              2021-06-22  完善注释说明
 **    
************************************************************************************************************************************/
#include <stm32f10x.h>  
#include <stdio.h>
#include "sys.h"
#include "string.h"       // C标准库头文件: 字符数组常用：strcpy()、strncpy()、strcmp()、strlen()、strnset()





/*****************************************************************************
 ** 移植配置
****************************************************************************/
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
    




/*****************************************************************************
 ** 声明全局函数
****************************************************************************/
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

















