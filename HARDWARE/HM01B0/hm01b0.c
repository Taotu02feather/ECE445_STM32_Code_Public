#include "sys.h"
//#include "ov7725.h"
//#include "ov7725config.h"	  
#include "delay.h"
#include "usart.h"			 
#include "sccb.h"	
#include "hm01b0.h"
struct senosr_reg* regs_list=hm01b0_324x244 ;
u8 countiic;
u8 hm01b0_Init(void)
{
	u16 i=0;
	u16 reg=0;
	//����IO
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO, ENABLE);//ʹ����ض˿�ʱ��
 
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8; 	//PA8 ���� ����  ֡ͬ���ź�
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOA,GPIO_Pin_8);
		
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;	 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4);	

	
	GPIO_InitStructure.GPIO_Pin  = 0xf; //PC0~3 ���� ����  D0-D3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	 
	
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��дreset
// 	GPIO_Init(GPIOD, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOD,GPIO_Pin_6);
	
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14|GPIO_Pin_15;  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_Init(GPIOG, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOG,GPIO_Pin_14|GPIO_Pin_15);
//	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//SWD
 
	SCCB_Init();        		//��ʼ��SCCB ��IO��	
// 	if(SCCB_WR_Reg(0x12,0x80))return 1;	//��λOV7725
//	delay_ms(50); 
//	reg=SCCB_RD_Reg(0X1c);		//��ȡ����ID �߰�λ
//	reg<<=8;
//	reg|=SCCB_RD_Reg(0X1d);		//��ȡ����ID �Ͱ�λ
//	if(reg!=OV7725_MID)
//	{
//		printf("MID:%d\r\n",reg);
//		return 1;
//	}
//	reg=SCCB_RD_Reg(0X0a);		//��ȡ����ID �߰�λ
//	reg<<=8;
//	reg|=SCCB_RD_Reg(0X0b);		//��ȡ����ID �Ͱ�λ
//	if(reg!=OV7725_PID)
//	{
//		printf("HID:%d\r\n",reg);
//		return 2;
//	}   
 	//��ʼ�� OV7725,����QVGA�ֱ���(320*240)  
	countiic=0;
	while (1) {
		u16 reg = regs_list->reg;
		u8 value = regs_list->val;
		
		if (reg == 0xFFFF && value == 0xFF) {
			break;
		}
		//printf("reg: 0x%04x , val: 0x%02x\r\n",reg, value);
		if(SCCB_WR_Reg(reg, value)){
			//return 0x01;
		}
		
		regs_list++;
		countiic++;
		delay_ms(10); //GJW 20230823
	}
//	for(i=0;i<sizeof(ov7725_init_reg_tb1)/sizeof(ov7725_init_reg_tb1[0]);i++)
//	{								
//	   	SCCB_WR_Reg(ov7725_init_reg_tb1[i][0],ov7725_init_reg_tb1[i][1]);
// 	} 
  	return 0x00; 	//ok
} 

ErrorStatus HSEStartUpStatus;
u8 Clock_mco_init(void){
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if (HSEStartUpStatus == SUCCESS) {
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		RCC_MCOConfig(RCC_MCO_HSE);
		return 0x00;
	}
	return 0x01;
	
}