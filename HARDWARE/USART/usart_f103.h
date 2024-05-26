#ifndef __USART_H
#define __USART_H
/***********************************************************************************************************************************
 **���������ӡ�  ħŮ�Ƽ�    https://demoboard.taobao.com
 **�����·���  QȺ�ļ���   887199504
 ***********************************************************************************************************************************
 **���ļ����ơ�  usart.h
 **���ļ����ܡ�  ���ļ��򻯵�USART�ĳ�ʼ�������������ա������ܺ�������ʼ������ú�������ʹ�ã�
 **
 **������ƽ̨��  STM32F103 + ��׼��v3.5 + keil5
 **
 **��ʹ��˵����  �� ֲ ������USART�ļ�����c��h�ļ���Ŀ���ļ��У���ӵ����̹�����, ����ļ�·��, ��� #include "usart.h����
 **              ��ʼ��: ���� USARTx_Init(������), ���������������ż�ʱ�����ã� 
 **              �� �� : �ַ���-USARTx_printf (char* stringTemp)
 **                      ����  -USARTx_SendData (uint8_t* buf, uint8_t cnt);
 **              �� �� : ��ʽ1 -�ⲿ�����ж�xUSART.USARTxRecivedFlag==��,Ȼ����xUSART.USARTxRecivedBuffer[256]�����ݣ�
 **                      ��ʽ2 -���ú���vUSARTx_GetBuffer(u8* buf, u8* cnt); 
 **                            
 **
 **�����¼�¼��  2020-09-02  �ļ�������USART1�����жϡ������жϡ������жϡ�DMA�շ�
 **              2021-06-04  USART1��2��3��UART4��5 ���շ����ƴ���
 **              2021-06-09  �����ļ���ʽ��ע��  
 **              2021-06-22  ����ע��˵��
 **    
************************************************************************************************************************************/
#include <stm32f10x.h>  
#include <stdio.h>
#include "sys.h"
#include "string.h"       // C��׼��ͷ�ļ�: �ַ����鳣�ã�strcpy()��strncpy()��strcmp()��strlen()��strnset()





/*****************************************************************************
 ** ��ֲ����
****************************************************************************/
#define USARTx_DEBUG         USART1    // �����ض���printf, ʹprintfͨ��USARTx��������





/*****************************************************************************
 ** ȫ�ֱ��� 
****************************************************************************/
typedef struct 
{
    uint8_t  USART1InitOK;             // ��ʼ�����; 0=δ��ʼ��, 1=�ѳ�ʼ��
    uint8_t  USART1RecivedFlag;        // ���յ��µ�һ֡���ݣ�0-�ޣ�1-�������ݣ��ڴ������ݺ󣬱��Ϊ0
    uint8_t  USART1RecivedCNT;         // ���յ����ٸ��ֽ�����
    uint8_t  USART1RecivedBuffer[256]; // ���յ����ݵĻ���
    
    uint8_t  USART2InitOK;             // ��ʼ�����; 0=δ��ʼ��, 1=�ѳ�ʼ��
    uint8_t  USART2RecivedFlag;        // ���յ��µ�һ֡���ݣ�0-�ޣ�1-�������ݣ��ڴ������ݺ󣬱��Ϊ0
    uint8_t  USART2RecivedCNT;         // ���յ����ٸ��ֽ�����
    uint8_t  USART2RecivedBuffer[256]; // ���յ����ݵĻ���
    
    uint8_t  USART3InitOK;             // ��ʼ�����; 0=δ��ʼ��, 1=�ѳ�ʼ��
    uint8_t  USART3RecivedFlag;        // ���յ��µ�һ֡���ݣ�0-�ޣ�1-�������ݣ��ڴ������ݺ󣬱��Ϊ0
    uint8_t  USART3RecivedCNT;         // ���յ����ٸ��ֽ�����
    uint8_t  USART3RecivedBuffer[256]; // ���յ����ݵĻ��� 
    
    uint8_t  UART4InitOK;              // ��ʼ�����; 0=δ��ʼ��, 1=�ѳ�ʼ��
    uint8_t  UART4RecivedFlag;         // ���յ��µ�һ֡���ݣ�0-�ޣ�1-�������ݣ��ڴ������ݺ󣬱��Ϊ0
    uint8_t  UART4RecivedCNT;          // ���յ����ٸ��ֽ�����
    uint8_t  UART4RecivedBuffer[256];  // ���յ����ݵĻ���
    
    uint8_t  UART5InitOK;              // ��ʼ�����; 0=δ��ʼ��, 1=�ѳ�ʼ��
    uint8_t  UART5RecivedFlag;         // ���յ��µ�һ֡���ݣ�0-�ޣ�1-�������ݣ��ڴ������ݺ󣬱��Ϊ0
    uint8_t  UART5RecivedCNT;          // ���յ����ٸ��ֽ�����
    uint8_t  UART5RecivedBuffer[256];  // ���յ����ݵĻ���
    
    uint8_t  testCNT;                  // �����ڲ���
    
}xUSATR_TypeDef;

extern xUSATR_TypeDef  xUSART;         // ����Ϊȫ�ֱ���,�����¼��Ϣ��״̬
    




/*****************************************************************************
 ** ����ȫ�ֺ���
****************************************************************************/
// USART1
void  USART1_Init (uint32_t baudrate);                          // ��ʼ�����ڵ�GPIO��ͨ�Ų������á��ж����ȼ�; (�����ʿ��衢8λ���ݡ���У�顢1��ֹͣλ)
void  USART1_GetBuffer (uint8_t* buffer, uint8_t* cnt);         // ��ȡ���յ�������
void  USART1_SendData (uint8_t* buf, uint8_t cnt);              // ͨ���жϷ������ݣ��ʺϸ�������
void  USART1_printf (char* stringTemp);                         // ͨ���жϷ����ַ������ʺ��ַ�����������256�������ڵ�
void  USART1_printfForDMA(char* stringTemp) ;                   // ͨ��DMA�������ݣ��ʺ�һ�ι������������ر����ַ�����ʡ��ռ���жϵ�ʱ��
// USART2
void  USART2_Init (uint32_t baudrate);                          // ��ʼ�����ڵ�GPIO��ͨ�Ų������á��ж����ȼ�; (�����ʿ��衢8λ���ݡ���У�顢1��ֹͣλ)
void  USART2_GetBuffer (uint8_t* buffer, uint8_t* cnt);         // ��ȡ���յ�������
void  USART2_SendData (uint8_t* buf, uint8_t cnt);              // ͨ���жϷ������ݣ��ʺϸ�������
void  USART2_printf (char* stringTemp);                         // ͨ���жϷ����ַ������ʺ��ַ�����������256�������ڵ�
// USART3
void  USART3_Init (uint32_t baudrate);                          // ��ʼ�����ڵ�GPIO��ͨ�Ų������á��ж����ȼ�; (�����ʿ��衢8λ���ݡ���У�顢1��ֹͣλ)
void  USART3_GetBuffer (uint8_t* buffer, uint8_t* cnt);         // ��ȡ���յ�������
void  USART3_SendData (uint8_t* buf, uint8_t cnt);              // ͨ���жϷ������ݣ��ʺϸ�������
void  USART3_printf (char* stringTemp);                         // ͨ���жϷ����ַ������ʺ��ַ�����������256�������ڵ�
// USART4
void  UART4_Init (uint32_t baudrate);                           // ��ʼ�����ڵ�GPIO��ͨ�Ų������á��ж����ȼ�; (�����ʿ��衢8λ���ݡ���У�顢1��ֹͣλ)
void  UART4_GetBuffer (uint8_t* buffer, uint8_t* cnt);          // ��ȡ���յ�������
void  UART4_SendData (uint8_t* buf, uint8_t cnt);               // ͨ���жϷ������ݣ��ʺϸ�������
void  UART4_printf (char* stringTemp);                          // ͨ���жϷ����ַ������ʺ��ַ�����������256�������ڵ�
// USART5
void  UART5_Init (uint32_t baudrate);                           // ��ʼ�����ڵ�GPIO��ͨ�Ų������á��ж����ȼ�; (�����ʿ��衢8λ���ݡ���У�顢1��ֹͣλ)
void  UART5_GetBuffer (uint8_t* buffer, uint8_t* cnt);          // ��ȡ���յ�������
void  UART5_SendData (uint8_t* buf, uint8_t cnt);               // ͨ���жϷ������ݣ��ʺϸ�������
void  UART5_printf (char* stringTemp);                          // ͨ���жϷ����ַ������ʺ��ַ�����������256�������ڵ�


#endif

















