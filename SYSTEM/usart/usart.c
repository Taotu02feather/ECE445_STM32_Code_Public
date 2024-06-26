#include "sys.h"
#include "usart.h"	
#include "core_cm3.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
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
////////////////////////////////////////////////////////////////////////////////// 	  
 
xUSATR_TypeDef  xUSART;         // 声明为全局变量,方便记录信息、状态
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#if 1
//#pragma import(__use_no_semihosting)             
////标准库需要的支持函数                 
//struct __FILE 
//{ 
//	int handle; 

//}; 

//FILE __stdout;       
////定义_sys_exit()以避免使用半主机模式    
//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 
////重定义fputc函数 
//int fputc(int ch, FILE *f)
//{      
//	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
//    USART1->DR = (u8) ch;      
//	return ch;
//}
//#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
//#if EN_USART1_RX   //如果使能了接收
////串口1中断服务程序
////注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
////接收状态
////bit15，	接收完成标志
////bit14，	接收到0x0d
////bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
//  
//void uart_init(u32 bound){
//  //GPIO端口设置
//  GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
//  
//	//USART1_TX   GPIOA.9
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
//   
//  //USART1_RX	  GPIOA.10初始化
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

//  //Usart1 NVIC 配置
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//  
//   //USART 初始化设置

//	USART_InitStructure.USART_BaudRate = bound;//串口波特率
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

//  USART_Init(USART1, &USART_InitStructure); //初始化串口1
//  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
//  USART_Cmd(USART1, ENABLE);                    //使能串口1 

//}

//void USART1_IRQHandler(void)                	//串口1中断服务程序
//	{
//	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//		{
//		Res =USART_ReceiveData(USART1);	//读取接收到的数据
//		
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//			{
//			if(USART_RX_STA&0x4000)//接收到了0x0d
//				{
//				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//				else USART_RX_STA|=0x8000;	//接收完成了 
//				}
//			else //还没收到0X0D
//				{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//					{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//					}		 
//				}
//			}   		 
//     } 
//#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntExit();  											 
//#endif
//} 
//	
//#endif


//////////////////////////////////////////////////////////////   USART-1   //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * 函  数： vUSART1_Init
 * 功  能： 初始化USART1的GPIO、通信参数配置、中断优先级 
 *          (8位数据、无校验、1个停止位)
 * 参  数： uint32_t baudrate  通信波特率
 * 返回值： 无
 ******************************************************************************/  
void USART1_Init(u32 baudrate)
{   
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;    
    
    // 时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;                           // 使能USART1时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                             // 使能GPIOA时钟

    // GPIO_TX引脚配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                // TX引脚，配置为复用推挽工作模式
    GPIO_Init (GPIOA, &GPIO_InitStructure);
    // GPIO_RX引脚配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING  ;        // RX引脚，由于板子上为一主多从电路，故选择复用开漏模式
    GPIO_Init (GPIOA, &GPIO_InitStructure);
    

    // 中断配置
    NVIC_InitStructure .NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure .NVIC_IRQChannelPreemptionPriority=2 ;       // 抢占优先级
    NVIC_InitStructure .NVIC_IRQChannelSubPriority = 2;                // 子优先级
    NVIC_InitStructure .NVIC_IRQChannelCmd = ENABLE;                // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                              
    
    //USART 初始化设置
    USART_DeInit(USART1); 
    USART_InitStructure.USART_BaudRate   = baudrate;                // 串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     // 字长为8位数据格式
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;        // 一个停止位
    USART_InitStructure.USART_Parity     = USART_Parity_No;         // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 使能收、发模式
    USART_Init(USART1, &USART_InitStructure);                       // 初始化串口
    
    USART_ITConfig(USART1, USART_IT_TXE , DISABLE );
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                  // 使能接受中断
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);                  // 使能空闲中断
    
    USART_Cmd(USART1, ENABLE);                                      // 使能串口, 开始工作  
   
    printf("\r\r\r=========== 魔女开发板 STM32F103 外设初始报告 ===========\r");      
    printf("USART1初始化配置      接收中断、空闲中断, 发送中断\r");    
}

/******************************************************************************
 * 函  数： USART1_IRQHandler
 * 功  能： USART1的接收中断、空闲中断、发送中断
 * 参  数： 无
 * 返回值： 无
 ******************************************************************************/
static u8 U1TxBuffer[256] ;    // 用于中断发送：环形缓冲区，256个字节
static u8 U1TxCounter = 0 ;    // 用于中断发送：标记已发送的字节数(环形)
static u8 U1TxCount   = 0 ;    // 用于中断发送：标记将要发送的字节数(环形)

void USART1_IRQHandler(void)           
{     
    static u8 cnt=0;
    static u8 RxTemp[256];
    
    // 接收中断
    if(USART1->SR & (1<<5))
    {
        RxTemp[cnt++] = USART1->DR ;                        // 读取数据寄存器值；注意：读取DR时自动清零中断位；
    }
    
    // 空闲中断, 用于配合接收中断，可判断一帧数据接收完成
    if(USART1->SR & (1<<4))                                 // 检查IDLE中断标志
    {         
        memcpy(xUSART.USART1RecivedBuffer , RxTemp , 256);  // 临时数据转存为全局数据, 等待处理，注意：复制的是整个数组，包括0值，以方便字符串数据
        xUSART.USART1RecivedFlag = 1;                       // 标记；外部程序通过检查xUSARTFlag.USART_2_Recived是否等于1, 可判断是否有新一帧数据   
        xUSART.USART1RecivedCNT  = cnt;
        cnt=0;  
        memset(RxTemp ,0, 256);                             // 临时数据空零,准备下一次的接收           
        USART1 ->SR;  USART1 ->DR;                          // 清零IDLE中断标志位!! 序列清零，顺序不能错!!   

        /********************************************************************************
         方式1：可在这里调用外部函数，处理接收到数据(不推荐)
         方式2：可在外部判断USARTxRecivedFla==1，然后处理数据区xUSART.USARTxRecivedBuffer  
         禁 止：不可在中断里调用printf等不可重入函数!!!
         示 例: 下面这一行代码, 只作示例, 输出到上位机以观察所收到的最新一帧数据, 可删除
        *********************************************************************************/
        //USART1_printf((char*)xUSART.USART1RecivedBuffer);  // 这行代码只作示例，输出USART1收到的最新一帧数据，可删除
     }     

    // 发送中断
    if ((USART1->SR & 1<<7) && (USART1->CR1 & 1<<7))        // 检查中断来源:TXE(发送数据寄存器空)
    {                
        USART1->DR = U1TxBuffer[U1TxCounter++];             // 读取数据寄存器值；注意：读取DR时自动清零中断位；
        
        if(U1TxCounter == U1TxCount )
            USART1->CR1 &= ~(1<<7);                         // 已发送完成，关闭发送缓冲区空置中断 TXEIE
    }    
}  

/******************************************************************************
 * 函  数： vUSART1_GetBuffer
 * 功  能： 获取UART所接收到的数据
 * 参  数： uint8_t* buffer   数据存放缓存地址
 *          uint8_t* cnt      接收到的字节数 
 * 返回值： 无
 ******************************************************************************/  
void USART1_GetBuffer(u8* buffer, u8* cnt)
{    
    if(xUSART.USART1RecivedFlag == 0)    return; 
    
    memcpy(buffer, xUSART.USART1RecivedBuffer, xUSART.USART1RecivedCNT );
    *cnt = xUSART.USART1RecivedCNT;
    
    xUSART.USART1RecivedFlag =0;   
}

/******************************************************************************
 * 函  数： vUSART1_SendData
 * 功  能： UART通过中断发送数据,适合各种数据类型
 *         【适合场景】本函数可发送各种数据，而不限于字符串，如int,char
 *         【不 适 合】注意环形缓冲区容量256字节，如果发送频率太高，注意波特率
 * 参  数： uint8_t* buffer   需发送数据的首地址
 *          uint8_t  cnt      发送的字节数 ，限于中断发送的缓存区大小，不能大于256个字节
 * 返回值：
 ******************************************************************************/  
void USART1_SendData(u8* buf, u8 cnt)
{
	u8 i;
    for(i=0; i<cnt; i++) 
        U1TxBuffer[U1TxCount++] = buf[i];
     
    if((USART1->CR1 & 1<<7) == 0 )         // 检查发送缓冲区空置中断(TXEIE)是否已打开
        USART1->CR1 |= 1<<7;             
}

/******************************************************************************
 * 函  数： vUSART1_printf
 * 功  能： UART通过中断发送输出字符串,无需输入数据长度
 *         【适合场景】字符串，长度<=256字节
 *         【不 适 合】int,float等数据类型
 * 参  数： char* stringTemp   需发送数据的缓存首地址
 * 返回值： 元
 ******************************************************************************/  
void USART1_printf(char* stringTemp)
{
    u16 num=0;                                   // 字符串长度
    char* t=stringTemp ;                         // 用于配合计算发送的数量    
    while(*t++ !=0)  num++;                      // 计算要发送的数目，这步比较耗时，测试发现每多6个字节，增加1us，单位：8位      
    USART1_SendData((u8*)stringTemp, num+1);     // 调用函数完成发送，num+1：字符串以0结尾，需多发一个:0   
}

/******************************************************************************
 * 函  数： vUSART1_printfForDMA
 * 功  能： UART通过DMA发送数据，省了占用中断的时间
 *         【适合场景】字符串，字节数非常多，
 *         【不 适 合】1:只适合发送字符串，不适合发送可能含0的数值类数据; 2-时间间隔要足够
 * 参  数： char strintTemp  要发送的字符串首地址
 * 返回值： 无
 ******************************************************************************/  
void USART1_printfForDMA(char* stringTemp) 
{
    static u8 Flag_DmaTxInit=0;                 // 用于标记是否已配置DMA发送
    u32   num = 0;                              // 发送的数量，注意发送的单位不是必须8位的    
    char* t =stringTemp ;                       // 用于配合计算发送的数量    
    
    while(*t++ !=0)  num++;                     // 计算要发送的数目，这步比较耗时，测试发现每多6个字节，增加1us，单位：8位           

    while(DMA1_Channel4->CNDTR > 0);            // 重要：如果DMA还在进行上次发送，就等待; 得进完成中断清标志，F4不用这么麻烦，发送完后EN自动清零
    if( Flag_DmaTxInit == 0)                    // 是否已进行过USAART_TX的DMA传输配置
    {   
        Flag_DmaTxInit  = 1;                    // 设置标记，下次调用本函数就不再进行配置了
        USART1 ->CR3   |= 1<<7;                 // 使能DMA发送
        RCC->AHBENR    |= 1<<0;                    // 开启DMA1时钟  [0]DMA1   [1]DMA2        
 
        DMA1_Channel4->CCR   = 0;               // 失能， 清0整个寄存器, DMA必须失能才能配置
        DMA1_Channel4->CNDTR = num;                // 传输数据量   
        DMA1_Channel4->CMAR  = (u32)stringTemp; // 存储器地址 
        DMA1_Channel4->CPAR  = (u32)&USART1->DR;// 外设地址      

        DMA1_Channel4->CCR |= 1<<4;              // 数据传输方向   0:从外设读   1:从存储器读
        DMA1_Channel4->CCR |= 0<<5;              // 循环模式       0:不循环     1：循环
        DMA1_Channel4->CCR |= 0<<6;             // 外设地址非增量模式
        DMA1_Channel4->CCR |= 1<<7;              // 存储器增量模式
        DMA1_Channel4->CCR |= 0<<8;              // 外设数据宽度为8位
        DMA1_Channel4->CCR |= 0<<10;             // 存储器数据宽度8位
        DMA1_Channel4->CCR |= 0<<12;             // 中等优先级
        DMA1_Channel4->CCR |= 0<<14;             // 非存储器到存储器模式    
    }    
    DMA1_Channel4->CCR  &= ~((u32)(1<<0));      // 失能，DMA必须失能才能配置
    DMA1_Channel4->CNDTR = num;                    // 传输数据量
    DMA1_Channel4->CMAR  = (u32)stringTemp;     // 存储器地址      
    DMA1_Channel4->CCR  |= 1<<0;                // 开启DMA传输   
} 


//////////////////////////////////////////////////////////////   USART-2   //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * 函  数： vUSART2_Init
 * 功  能： 初始化USART的GPIO、通信参数配置、中断优先级 
 *          (8位数据、无校验、1个停止位)
 * 参  数： uint32_t baudrate  通信波特率
 * 返回值： 无
 ******************************************************************************/  
void USART2_Init(u32 baudrate)
{   
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;    
    
    // 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;                           // 使能USART1时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                             // 使能GPIOA时钟

    // GPIO_TX引脚配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                // TX引脚，配置为复用推挽工作模式
    GPIO_Init (GPIOA, &GPIO_InitStructure);
    // GPIO_RX引脚配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;         // RX引脚，配置为浮空输入工作模式
    GPIO_Init (GPIOA, &GPIO_InitStructure);
    
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // 中断配置
    NVIC_InitStructure .NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure .NVIC_IRQChannelPreemptionPriority=2 ;       // 抢占优先级
    NVIC_InitStructure .NVIC_IRQChannelSubPriority = 2;                // 子优先级
    NVIC_InitStructure .NVIC_IRQChannelCmd = ENABLE;                // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                              
    
    //USART 初始化设置
    //USART_DeInit(USART2); 
    USART_InitStructure.USART_BaudRate   = baudrate;                // 串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     // 字长为8位数据格式
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;        // 一个停止位
    USART_InitStructure.USART_Parity     = USART_Parity_No;         // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    // 使能收、发模式
    USART_Init(USART2, &USART_InitStructure);                       // 初始化串口
    
    USART_ITConfig(USART2, USART_IT_TXE , DISABLE );
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                  // 使能接受中断
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);                  // 使能空闲中断
    
    USART_Cmd(USART2, ENABLE);                                      // 使能串口, 开始工作  
      
    printf("\rUSART2初始化配置      接收中断、空闲中断, 发送中断\r");    
}

/******************************************************************************
 * 函  数： USART2_IRQHandler
 * 功  能： USART2的接收中断、空闲中断、发送中断
 * 参  数： 无
 * 返回值： 无
 ******************************************************************************/
static u8 U2TxBuffer[256] ;    // 用于中断发送：环形缓冲区，256个字节
static u8 U2TxCounter = 0 ;    // 用于中断发送：标记已发送的字节数(环形)
static u8 U2TxCount   = 0 ;    // 用于中断发送：标记将要发送的字节数(环形)

void USART2_IRQHandler(void)           
{     
    static u8 cnt=0;
    static u8 RxTemp[256];
    
    // 接收中断
    if(USART2->SR & (1<<5))
    {
        RxTemp[cnt++] = USART2->DR ;                        // 读取数据寄存器值；注意：读取DR时自动清零中断位；
    }
    
    // 空闲中断, 用于配合接收中断，可判断一帧数据接收完成
    if(USART2->SR & (1<<4))                                 // 检查IDLE中断标志
    {         
        memcpy(xUSART.USART2RecivedBuffer , RxTemp , 256);  // 临时数据转存为全局数据, 等待处理，注意：复制的是整个数组，包括0值，以方便字符串数据
        xUSART.USART2RecivedFlag = 1;                       // 标记；外部程序通过检查xUSARTFlag.USART_2_Recived是否等于1, 可判断是否有新一帧数据   
        xUSART.USART2RecivedCNT  = cnt;
        cnt=0;  
        memset(RxTemp ,0, 256);                             // 临时数据空零,准备下一次的接收 
        USART2 ->SR;  USART2 ->DR;                          // 清零IDLE中断标志位!! 序列清零，顺序不能错!!  
			if(xUSART.USART2RecivedBuffer[0]=='r'){
						__set_FAULTMASK(1);
						NVIC_SystemReset();
				}
        /********************************************************************************
         方式1：可在这里调用外部函数，处理接收到数据(不推荐)
         方式2：可在外部判断USARTxRecivedFla==1，然后处理数据区xUSART.USARTxRecivedBuffer  
         禁 止：不可在中断里调用printf等不可重入函数!!!
         示 例: 下面这一行代码, 只作示例, 输出到上位机以观察所收到的最新一帧数据, 可删除
        *********************************************************************************/
        USART1_printf((char*)xUSART.USART2RecivedBuffer);  // 这行代码只作示例，输出USART2收到的最新一帧数据，可删除   
     }     

    // 发送中断
    if ((USART2->SR & 1<<7) && (USART2->CR1 & 1<<7))        // 检查中断来源:TXE(发送数据寄存器空)
    {                
        USART2->DR = U2TxBuffer[U2TxCounter++];             // 读取数据寄存器值；注意：读取DR时自动清零中断位；
        
        if(U2TxCounter == U2TxCount )
            USART2->CR1 &= ~(1<<7);                         // 已发送完成，关闭发送缓冲区空置中断 TXEIE
    }    
}  

/******************************************************************************
 * 函  数： vUSART2_GetBuffer
 * 功  能： 获取UART所接收到的数据
 * 参  数： uint8_t* buffer   数据存放缓存地址
 *          uint8_t* cnt      接收到的字节数 
 * 返回值： 接收到的字节数
 ******************************************************************************/  
void USART2_GetBuffer(u8* buffer, u8* cnt)
{    
    if(xUSART.USART2RecivedFlag == 0)    return ; 
    
    memcpy(buffer, xUSART.USART2RecivedBuffer, xUSART.USART2RecivedCNT );
    *cnt = xUSART.USART2RecivedCNT;
    
    xUSART.USART2RecivedFlag =0;    
}

/******************************************************************************
 * 函  数： vUSART2_SendData
 * 功  能： UART通过中断发送数据,适合各种数据类型
 *         【适合场景】本函数可发送各种数据，而不限于字符串，如int,char
 *         【不 适 合】注意环形缓冲区容量256字节，如果发送频率太高，注意波特率
 * 参  数： uint8_t* buffer   需发送数据的首地址
 *          uint8_t  cnt      发送的字节数 ，限于中断发送的缓存区大小，不能大于256个字节
 * 返回值：
 ******************************************************************************/  
void USART2_SendData(u8* buf, u8 cnt)
{
	u8 i;
    for( i=0; i<cnt; i++){ 
        U2TxBuffer[U2TxCount++] = buf[i];
		}
    if((USART2->CR1 & 1<<7) == 0 ){         // 检查发送缓冲区空置中断(TXEIE)是否已打开
        USART2->CR1 |= 1<<7;  
		}           
}

/******************************************************************************
 * 函  数： vUSART2_printf
 * 功  能： UART通过中断发送输出字符串,无需输入数据长度
 *         【适合场景】字符串，长度<=256字节
 *         【不 适 合】int,float等数据类型
 * 参  数： char* stringTemp   需发送数据的缓存首地址
 * 返回值： 元
 ******************************************************************************/  
void USART2_printf(char* stringTemp)
{
    u16 num=0;                                   // 字符串长度
    char* t=stringTemp ;                         // 用于配合计算发送的数量    
    while(*t++ !=0)  num++;                      // 计算要发送的数目，这步比较耗时，测试发现每多6个字节，增加1us，单位：8位      
    USART2_SendData((u8*)stringTemp, num+1);     // 调用函数完成发送，num+1：字符串以0结尾，需多发一个:0   
}





//////////////////////////////////////////////////////////////   USART-3   //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * 函  数： vUSART3_Init
 * 功  能： 初始化USART的GPIO、通信参数配置、中断优先级 
 *          (8位数据、无校验、1个停止位)
 * 参  数： uint32_t baudrate  通信波特率
 * 返回值： 无
 ******************************************************************************/  
void USART3_Init(u32 baudrate)
{   
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;    
    
    // 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;                           // 使能USART1时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;                             // 使能GPIOA时钟

    // GPIO_TX引脚配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                // TX引脚，配置为复用推挽工作模式
    GPIO_Init (GPIOB, &GPIO_InitStructure);
    // GPIO_RX引脚配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;         // RX引脚，配置为浮空输入工作模式
    GPIO_Init (GPIOB, &GPIO_InitStructure);
    
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // 中断配置
    NVIC_InitStructure .NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure .NVIC_IRQChannelPreemptionPriority=2 ;       // 抢占优先级
    NVIC_InitStructure .NVIC_IRQChannelSubPriority = 2;             // 子优先级
    NVIC_InitStructure .NVIC_IRQChannelCmd = ENABLE;                // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                              
    
    //USART 初始化设置
    USART_DeInit(USART3); 
    USART_InitStructure.USART_BaudRate   = baudrate;                // 串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     // 字长为8位数据格式
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;        // 一个停止位
    USART_InitStructure.USART_Parity     = USART_Parity_No;         // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 使能收、发模式
    USART_Init(USART3, &USART_InitStructure);                       // 初始化串口
    
    USART_ITConfig(USART3, USART_IT_TXE , DISABLE );
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);                  // 使能接受中断
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);                  // 使能空闲中断
    
    USART_Cmd(USART3, ENABLE);                                      // 使能串口, 开始工作  
      
    printf("\rUSART3初始化配置      接收中断、空闲中断, 发送中断\r");    
}

/******************************************************************************
 * 函  数： USART3_IRQHandler
 * 功  能： USART的接收中断、空闲中断、发送中断
 * 参  数： 无
 * 返回值： 无
 ******************************************************************************/
static u8 U3TxBuffer[256] ;    // 用于中断发送：环形缓冲区，256个字节
static u8 U3TxCounter = 0 ;    // 用于中断发送：标记已发送的字节数(环形)
static u8 U3TxCount   = 0 ;    // 用于中断发送：标记将要发送的字节数(环形)

void USART3_IRQHandler(void)           
{     
    static u8 cnt=0;
    static u8 RxTemp[256];
    
    // 接收中断
    if(USART3->SR & (1<<5))
    {
        RxTemp[cnt++] = USART3->DR ;                        // 读取数据寄存器值；注意：读取DR时自动清零中断位；
    }
    
    // 空闲中断, 用于配合接收中断，可判断一帧数据接收完成
    if(USART3->SR & (1<<4))                                 // 检查IDLE中断标志
    {         
        memcpy(xUSART.USART3RecivedBuffer , RxTemp , 256);  // 临时数据转存为全局数据, 等待处理，注意：复制的是整个数组，包括0值，以方便字符串数据
        xUSART.USART3RecivedFlag = 1;                       // 标记；外部程序通过检查xUSARTFlag.USART_2_Recived是否等于1, 可判断是否有新一帧数据   
        xUSART.USART3RecivedCNT  = cnt;
        cnt=0;  
        memset(RxTemp ,0, 256);                             // 临时数据空零,准备下一次的接收   
        USART3 ->SR;  USART3 ->DR;                          // 清零IDLE中断标志位!! 序列清零，顺序不能错!! 
        
        /********************************************************************************
         方式1：可在这里调用外部函数，处理接收到数据(不推荐)
         方式2：可在外部判断USARTxRecivedFla==1，然后处理数据区xUSART.USARTxRecivedBuffer  
         禁 止：不可在中断里调用printf等不可重入函数!!!
         示 例: 下面这一行代码, 只作示例, 输出到上位机以观察所收到的最新一帧数据, 可删除
        *********************************************************************************/
        USART1_printf((char*)xUSART.USART3RecivedBuffer);   // 这行代码只作示例，输出USART3收到的最新一帧数据，可删除  
     }     

    // 发送中断
    if ((USART3->SR & 1<<7) && (USART3->CR1 & 1<<7))        // 检查中断来源:TXE(发送数据寄存器空)
    {                
        USART3->DR = U3TxBuffer[U3TxCounter++];             // 读取数据寄存器值；注意：读取DR时自动清零中断位；
        
        if(U3TxCounter == U3TxCount )
            USART3->CR1 &= ~(1<<7);                         // 已发送完成，关闭发送缓冲区空置中断 TXEIE
    }    
}  

/******************************************************************************
 * 函  数： vUSART3_GetBuffer
 * 功  能： 获取UART所接收到的数据
 * 参  数： uint8_t* buffer   数据存放缓存地址
 *          uint8_t* cnt      接收到的字节数 
 * 返回值： 接收到的字节数
 ******************************************************************************/  
void USART3_GetBuffer(u8* buffer, u8* cnt)
{    
    if(xUSART.USART3RecivedFlag == 0)    return ; 
    
    memcpy(buffer, xUSART.USART3RecivedBuffer, xUSART.USART3RecivedCNT );
    *cnt = xUSART.USART3RecivedCNT;
    
    xUSART.USART3RecivedFlag =0;    
}

/******************************************************************************
 * 函  数： vUSART3_SendData
 * 功  能： UART通过中断发送数据,适合各种数据类型
 *         【适合场景】本函数可发送各种数据，而不限于字符串，如int,char
 *         【不 适 合】注意环形缓冲区容量256字节，如果发送频率太高，注意波特率
 * 参  数： uint8_t* buffer   需发送数据的首地址
 *          uint8_t  cnt      发送的字节数 ，限于中断发送的缓存区大小，不能大于256个字节
 * 返回值：
 ******************************************************************************/  
void USART3_SendData(u8* buf, u8 cnt)
{
	u8 i;
    for(i=0; i<cnt; i++) 
        U3TxBuffer[U3TxCount++] = buf[i];
     
    if((USART3->CR1 & 1<<7) == 0 )         // 检查发送缓冲区空置中断(TXEIE)是否已打开
        USART3->CR1 |= 1<<7;             
}

/******************************************************************************
 * 函  数： vUSART3_printf
 * 功  能： UART通过中断发送输出字符串,无需输入数据长度
 *         【适合场景】字符串，长度<=256字节
 *         【不 适 合】int,float等数据类型
 * 参  数： char* stringTemp   需发送数据的缓存首地址
 * 返回值： 元
 ******************************************************************************/  
void USART3_printf(char* stringTemp)
{
    u16 num=0;                                   // 字符串长度
    char* t=stringTemp ;                         // 用于配合计算发送的数量    
    while(*t++ !=0)  num++;                      // 计算要发送的数目，这步比较耗时，测试发现每多6个字节，增加1us，单位：8位      
    USART3_SendData((u8*)stringTemp, num+1);     // 调用函数完成发送，num+1：字符串以0结尾，需多发一个:0   
}




#ifdef STM32F10X_HD  // STM32F103R，及以上，才有UART4和UART5

//////////////////////////////////////////////////////////////   UART-4   //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * 函  数： vUART4_Init
 * 功  能： 初始化USART的GPIO、通信参数配置、中断优先级 
 *          (8位数据、无校验、1个停止位)
 * 参  数： uint32_t baudrate  通信波特率
 * 返回值： 无
 ******************************************************************************/  
void UART4_Init(u32 baudrate)
{   
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;    
    
    // 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_UART4EN;                            // 使能USART时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;                             // 使能GPIOA时钟

    // GPIO_TX引脚配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                // TX引脚，配置为复用推挽工作模式
    GPIO_Init (GPIOC, &GPIO_InitStructure);
    // GPIO_RX引脚配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;         // RX引脚，配置为浮空输入工作模式
    GPIO_Init (GPIOC, &GPIO_InitStructure);
    
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // 中断配置
    NVIC_InitStructure .NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure .NVIC_IRQChannelPreemptionPriority=2 ;       // 抢占优先级
    NVIC_InitStructure .NVIC_IRQChannelSubPriority = 2;                // 子优先级
    NVIC_InitStructure .NVIC_IRQChannelCmd = ENABLE;                // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                              
    
    //USART 初始化设置
    USART_DeInit(UART4); 
    USART_InitStructure.USART_BaudRate   = baudrate;                // 串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     // 字长为8位数据格式
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;        // 一个停止位
    USART_InitStructure.USART_Parity     = USART_Parity_No;         // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    // 使能收、发模式
    USART_Init(UART4, &USART_InitStructure);                        // 初始化串口
    
    USART_ITConfig(UART4, USART_IT_TXE , DISABLE );
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);                   // 使能接受中断
    USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);                   // 使能空闲中断
    
    USART_Cmd(UART4, ENABLE);                                       // 使能串口, 开始工作  
      
    printf("\rUART4 初始化配置      接收中断、空闲中断, 发送中断\r");    
}

/******************************************************************************
 * 函  数： UART4_IRQHandler
 * 功  能： USART2的接收中断、空闲中断、发送中断
 * 参  数： 无
 * 返回值： 无
 ******************************************************************************/
static u8 U4TxBuffer[256] ;    // 用于中断发送：环形缓冲区，256个字节
static u8 U4TxCounter = 0 ;    // 用于中断发送：标记已发送的字节数(环形)
static u8 U4TxCount   = 0 ;    // 用于中断发送：标记将要发送的字节数(环形)

void UART4_IRQHandler(void)           
{     
    static u8 cnt=0;
    static u8 RxTemp[256];
    
    // 接收中断
    if(UART4->SR & (1<<5))
    {
        RxTemp[cnt++] = UART4->DR ;                         // 读取数据寄存器值；注意：读取DR时自动清零中断位；
    }
    
    // 空闲中断, 用于配合接收中断，可判断一帧数据接收完成
    if(UART4->SR & (1<<4))                                  // 检查IDLE中断标志
    {         
        memcpy(xUSART.UART4RecivedBuffer , RxTemp , 256);   // 临时数据转存为全局数据, 等待处理，注意：复制的是整个数组，包括0值，以方便字符串数据
        xUSART.UART4RecivedFlag = 1;                        // 标记；外部程序通过检查xUSARTFlag.USART_2_Recived是否等于1, 可判断是否有新一帧数据   
        xUSART.UART4RecivedCNT  = cnt;
        cnt=0;  
        memset(RxTemp ,0, 256);                             // 临时数据空零,准备下一次的接收   
        UART4 ->SR;  UART4 ->DR;                            // 清零IDLE中断标志位!! 序列清零，顺序不能错!! 
        
        /********************************************************************************
         方式1：可在这里调用外部函数，处理接收到数据(不推荐)
         方式2：可在外部判断USARTxRecivedFla==1，然后处理数据区xUSART.USARTxRecivedBuffer  
         禁 止：不可在中断里调用printf等不可重入函数!!!
         示 例: 下面这一行代码, 只作示例, 输出到上位机以观察所收到的最新一帧数据, 可删除
        *********************************************************************************/
        USART1_printf((char*)xUSART.UART4RecivedBuffer);    // 这行代码只作示例，输出UART4收到的最新一帧数据，可删除    
     }     

    // 发送中断
    if ((UART4->SR & 1<<7) && (UART4->CR1 & 1<<7))          // 检查中断来源:TXE(发送数据寄存器空)
    {                
        UART4->DR = U4TxBuffer[U4TxCounter++];              // 读取数据寄存器值；注意：读取DR时自动清零中断位；
        
        if(U4TxCounter == U4TxCount )
            UART4->CR1 &= ~(1<<7);                          // 已发送完成，关闭发送缓冲区空置中断 TXEIE
    }    
}  

/******************************************************************************
 * 函  数： vUART4_GetBuffer
 * 功  能： 获取UART所接收到的数据
 * 参  数： uint8_t* buffer   数据存放缓存地址
 *          uint8_t* cnt      接收到的字节数 
 * 返回值： 接收到的字节数
 ******************************************************************************/  
void UART4_GetBuffer(u8* buffer, u8* cnt)
{    
    if(xUSART.UART4RecivedFlag == 0)    return ; 
    
    memcpy(buffer, xUSART.UART4RecivedBuffer, xUSART.UART4RecivedCNT );
    *cnt = xUSART.UART4RecivedCNT;
    
    xUSART.UART4RecivedFlag =0;    
}

/******************************************************************************
 * 函  数： vUART4_SendData
 * 功  能： UART通过中断发送数据,适合各种数据类型
 *         【适合场景】本函数可发送各种数据，而不限于字符串，如int,char
 *         【不 适 合】注意环形缓冲区容量256字节，如果发送频率太高，注意波特率
 * 参  数： uint8_t* buffer   需发送数据的首地址
 *          uint8_t  cnt      发送的字节数 ，限于中断发送的缓存区大小，不能大于256个字节
 * 返回值：
 ******************************************************************************/  
void UART4_SendData(u8* buf, u8 cnt)
{
	u8 i;
    for( i=0; i<cnt; i++) 
        U4TxBuffer[U4TxCount++] = buf[i];
     
    if((UART4->CR1 & 1<<7) == 0 )         // 检查发送缓冲区空置中断(TXEIE)是否已打开
        UART4->CR1 |= 1<<7;             
}

/******************************************************************************
 * 函  数： vUART4_printf
 * 功  能： UART通过中断发送输出字符串,无需输入数据长度
 *         【适合场景】字符串，长度<=256字节
 *         【不 适 合】int,float等数据类型
 * 参  数： char* stringTemp   需发送数据的缓存首地址
 * 返回值： 元
 ******************************************************************************/  
void UART4_printf(char* stringTemp)
{
    u16 num=0;                                   // 字符串长度
    char* t = stringTemp ;                       // 用于配合计算发送的数量    
    while(*t++ !=0)  num++;                      // 计算要发送的数目，这步比较耗时，测试发现每多6个字节，增加1us，单位：8位      
    UART4_SendData((u8*)stringTemp, num+1);      // 调用函数完成发送，num+1：字符串以0结尾，需多发一个:0   
}




//////////////////////////////////////////////////////////////   UART-4   //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * 函  数： vUART5_Init
 * 功  能： 初始化USART的GPIO、通信参数配置、中断优先级 
 *          (8位数据、无校验、1个停止位)
 * 参  数： uint32_t baudrate  通信波特率
 * 返回值： 无
 ******************************************************************************/  
void UART5_Init(u32 baudrate)
{   
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;    
    
    // 时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_UART5EN;                            // 使能USART1时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN | RCC_APB2ENR_IOPCEN;        // 使能GPIO时钟

    // GPIO_TX引脚配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                // TX引脚，配置为复用推挽工作模式
    GPIO_Init (GPIOC, &GPIO_InitStructure);
    // GPIO_RX引脚配置
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;         // RX引脚，配置为浮空输入工作模式
    GPIO_Init (GPIOD, &GPIO_InitStructure);
    
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // 中断配置
    NVIC_InitStructure .NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure .NVIC_IRQChannelPreemptionPriority=2 ;       // 抢占优先级
    NVIC_InitStructure .NVIC_IRQChannelSubPriority = 2;                // 子优先级
    NVIC_InitStructure .NVIC_IRQChannelCmd = ENABLE;                // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                              
    
    //USART 初始化设置
    USART_DeInit(UART5); 
    USART_InitStructure.USART_BaudRate   = baudrate;                // 串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     // 字长为8位数据格式
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;        // 一个停止位
    USART_InitStructure.USART_Parity     = USART_Parity_No;         // 无奇偶校验位
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    // 使能收、发模式
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(UART5, &USART_InitStructure);                        // 初始化串口
    
    USART_ITConfig(UART5, USART_IT_TXE , DISABLE );
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);                   // 使能接受中断
    USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);                   // 使能空闲中断
    
    USART_Cmd(UART5, ENABLE);                                       // 使能串口, 开始工作  
      
    printf("\rUART5 初始化配置      接收中断、空闲中断, 发送中断\r");    
}

/******************************************************************************
 * 函  数： UART5_IRQHandler
 * 功  能： USART2的接收中断、空闲中断、发送中断
 * 参  数： 无
 * 返回值： 无
 ******************************************************************************/
static u8 U5TxBuffer[256] ;    // 用于中断发送：环形缓冲区，256个字节
static u8 U5TxCounter = 0 ;    // 用于中断发送：标记已发送的字节数(环形)
static u8 U5TxCount   = 0 ;    // 用于中断发送：标记将要发送的字节数(环形)

void UART5_IRQHandler(void)           
{     
    static u8 cnt=0;
    static u8 RxTemp[256];
    
    // 接收中断
    if(UART5->SR & (1<<5))
    {
        RxTemp[cnt++] = UART5->DR ;                        // 读取数据寄存器值；注意：读取DR时自动清零中断位；
    }
    
    // 空闲中断, 用于配合接收中断，可判断一帧数据接收完成
    if(UART5->SR & (1<<4))                                 // 检查IDLE中断标志
    {         
        memcpy(xUSART.UART5RecivedBuffer , RxTemp , 256);  // 临时数据转存为全局数据, 等待处理，注意：复制的是整个数组，包括0值，以方便字符串数据
        xUSART.UART5RecivedFlag = 1;                       // 标记；外部程序通过检查xUSARTFlag.USART_2_Recived是否等于1, 可判断是否有新一帧数据   
        xUSART.UART5RecivedCNT  = cnt;
        cnt=0;  
        memset(RxTemp ,0, 256);                            // 临时数据空零,准备下一次的接收   
        UART5 ->SR;  UART5 ->DR;                           // 清零IDLE中断标志位!! 序列清零，顺序不能错!! 
        
        /********************************************************************************
         方式1：可在这里调用外部函数，处理接收到数据(不推荐)
         方式2：可在外部判断USARTxRecivedFla==1，然后处理数据区xUSART.USARTxRecivedBuffer  
         禁 止：不可在中断里调用printf等不可重入函数!!!
         示 例: 下面这一行代码, 只作示例, 输出到上位机以观察所收到的最新一帧数据, 可删除
        *********************************************************************************/
        USART1_printf((char*)xUSART.UART5RecivedBuffer);  // 这行代码只作示例，输出UART5收到的最新一帧数据，可删除 
     }     

    // 发送中断
    if ((UART5->SR & 1<<7) && (UART5->CR1 & 1<<7))         // 检查中断来源:TXE(发送数据寄存器空)
    {                
        UART5->DR = U5TxBuffer[U5TxCounter++];             // 读取数据寄存器值；注意：读取DR时自动清零中断位；
        
        if(U5TxCounter == U5TxCount )
            UART5->CR1 &= ~(1<<7);                         // 已发送完成，关闭发送缓冲区空置中断 TXEIE
    }    
}  

/******************************************************************************
 * 函  数： vUART5_GetBuffer
 * 功  能： 获取UART所接收到的数据
 * 参  数： uint8_t* buffer   数据存放缓存地址
 *          uint8_t* cnt      接收到的字节数 
 * 返回值： 接收到的字节数
 ******************************************************************************/  
void UART5_GetBuffer(u8* buffer, u8* cnt)
{    
    if(xUSART.UART5RecivedFlag == 0)    return ; 
    
    memcpy(buffer, xUSART.UART5RecivedBuffer, xUSART.UART5RecivedCNT );
    *cnt = xUSART.UART5RecivedCNT;
    
    xUSART.UART5RecivedFlag =0;    
}

/******************************************************************************
 * 函  数： vUART5_SendData
 * 功  能： UART通过中断发送数据,适合各种数据类型
 *         【适合场景】本函数可发送各种数据，而不限于字符串，如int,char
 *         【不 适 合】注意环形缓冲区容量256字节，如果发送频率太高，注意波特率
 * 参  数： uint8_t* buffer   需发送数据的首地址
 *          uint8_t  cnt      发送的字节数 ，限于中断发送的缓存区大小，不能大于256个字节
 * 返回值：
 ******************************************************************************/  
void UART5_SendData(u8* buf, u8 cnt)
{
  u8 i;
	for(i=0; i<cnt; i++) 
        U5TxBuffer[U5TxCount++] = buf[i];
     
    if((UART5->CR1 & 1<<7) == 0 )         // 检查发送缓冲区空置中断(TXEIE)是否已打开
        UART5->CR1 |= 1<<7;             
}

/******************************************************************************
 * 函  数： vUART5_printf
 * 功  能： UART通过中断发送输出字符串,无需输入数据长度
 *         【适合场景】字符串，长度<=256字节
 *         【不 适 合】int,float等数据类型
 * 参  数： char* stringTemp   需发送数据的缓存首地址
 * 返回值： 元
 ******************************************************************************/  
void UART5_printf(char* stringTemp)
{
    u16 num=0;                                   // 字符串长度
    char* t=stringTemp ;                         // 用于配合计算发送的数量    
    while(*t++ !=0)  num++;                      // 计算要发送的数目，这步比较耗时，测试发现每多6个字节，增加1us，单位：8位      
    UART5_SendData((u8*)stringTemp, num+1);      // 调用函数完成发送，num+1：字符串以0结尾，需多发一个:0   
}

#endif	



//////////////////////////////////////////////////////////////  printf   //////////////////////////////////////////////////////////////
/******************************************************************************
 * 功  能： printf函数支持代码
 *         【特别注意】加入以下代码, 使用printf函数时, 不再需要选择use MicroLIB     
 * 参  数： 
 * 返回值：
 * 备  注： 魔女开发板团队  资料存放Q群：887199504      最后修改_2020年07月15日
 ******************************************************************************/  
//加入以下代码,支持printf函数,而不需要选择use MicroLIB     
#pragma import(__use_no_semihosting)                
struct __FILE       { int handle; };         // 标准库需要的支持函数
FILE __stdout;                               // FILE 在stdio.h文件
void _sys_exit(int x) {    x = x; }             // 定义_sys_exit()以避免使用半主机模式

int fputc(int ch, FILE *f)                   // 重定向fputc函数，使printf的输出，由fputc输出到UART,  这里使用串口1(USART1)
{ 
    #if 1                                    // 方式1-使用常用的poll方式发送数据，比较容易理解，但等待耗时大  
        while((USARTx_DEBUG->SR & 0X40)==0); // 等待上一次串口数据发送完成 
        USARTx_DEBUG->DR = (u8) ch;          // 写DR,串口1将发送数据 
        return ch;
    #else                                    // 方式2-使用queue+中断方式发送数据; 无需像方式1那样等待耗时，但要借助已写好的函数、环形缓冲
        uint8_t c[1]={(uint8_t)ch};    
        if(USARTx_DEBUG == USART1)    vUSART1_SendData (c, 1);
        if(USARTx_DEBUG == USART2)    vUSART2_SendData (c, 1);
        if(USARTx_DEBUG == USART3)    vUSART3_SendData (c, 1);
        if(USARTx_DEBUG == UART4)     vUART4_SendData  (c, 1);
        if(USARTx_DEBUG == UART5)     vUART5_SendData  (c, 1);
        return ch;
    #endif    
}



