#include "USER_MPU6050.h"
#include "timer.h"
#include "Array.h"
#include "Control_System.h"
/************* Adding part Begin *****************/
//static float wRA[3][3];

//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{   	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
	USART_SendData(USART1,c);  
} 
//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}
//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	usart1_niming_report(0XA1,tbuf,12);//自定义帧,0XA1
}	
//通过串口1上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//清0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	usart1_niming_report(0XAF,tbuf,28);//飞控显示帧,0XAF
}  
 	

/************* Adding part End *****************/

int MPU_USER_Init(){ 
	int r;
	char r_string[10];
	r = 1;
	
	//uart_init(500000);	 	//串口初始化为500000
	//
	USART1_Init(500000);
	MPU_Init();
	while((r!=8)&&(r!=0))
 	{
		r = mpu_dmp_init();
		//  LCD_ShowString(20,20,200,16,12,"MPU6050 Int Error");
		//20,60,200,16,16
		if(LCD_en == 1){
			LCD_ShowNum(30,160,r,5,12);		//显示整数部分
			delay_ms(200);
			LCD_Fill(30,130,239,130+16,WHITE);
			delay_ms(200);
		}
		else if(LCD_en == 0){
			sprintf(r_string, " Ini %d \n", r);
			HC08_SendString(r_string);
			xUSART.USART1RecivedFlag=0; 
		}
		
	}  
	//mpu_set_dmp_state(1);	//使能DMP
	mpu_set_dmp_state(1);	//使能DMP
	return r;
}

int MPU6050_Report(u8 report, long* aac, float* wRA, long* aacw, short* gyro, float* angle){ 
	// now, the report is always on, which is 1
	// return 0正常
	// return 1 error
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short temper;					//温度	
	short temp;
	int line_num;		// 显示的行数
	float ARB[9];
	float wRB[9];
	float aac_f[3];
	float aacw_f[3];
	u8 key;
	report = 1;
	// put aacxyz to aac, gyroxyz to gyro,
	// and put pitch,roll,yaw to angle
	if(report){
		// key test
		/*
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			report=!report;
			if(report)LCD_ShowString(30,170,160,16,12,"KEY TEST1");
			else LCD_ShowString(30,170,160,16,12,"KEY TEST0");
		}
		// key test end
		*/
		//if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0){
			if(LCD_en==1){
				LCD_ShowString(20,60,200,16,12,"MPU DMP OK    ");
			}
			else if(LCD_en == 0){
				//HC08_SendString("MPU DMP OK \n");
				//xUSART.USART1RecivedFlag=0;  
			}
			
		}
		else{

			if(LCD_en==1){
				LCD_ShowString(20,60,200,16,12,"MPU DMP ERROR    ");
			}
			else if(LCD_en == 0){
				//HC08_SendString("MPU DMP ERROR \n");
				//xUSART.USART1RecivedFlag=0;  
			}
			return 1;
		}
		//mpu_dmp_get_data(&pitch,&roll,&yaw);  //计算欧拉角
		
		temper=MPU_Get_Temperature();	//得到温度值
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
		mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//用自定义帧发送加速度和陀螺仪原始数据
		usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
	}
		aac[0] = (long)aacx;
		aac[1] = (long)aacy;
		aac[2] = (long)aacz;
		aac_f[0] = aacx;
		aac_f[1] = aacy;
		aac_f[2] = aacz;
		gyro[0] = gyrox;
		gyro[1] = gyroy;
		gyro[2] = gyroz;
		// and put pitch,roll,yaw to angle
		angle[0] = pitch;
		angle[1] = roll;
		angle[2] = yaw;
		// get aac_world
		GetARB(ARB,angle);
		Matrix_33_33(wRB, wRA, ARB);
		Matrix_33_31(aacw_f, wRB, aac_f);
		aacw[0] = (long)aacw_f[0];
		aacw[1] = (long)aacw_f[1];
		aacw[2] = (long)aacw_f[2];
	return 0;
	
}
int MPU6050_Print(u8 report, long* aac, long* aacw, short* gyro, float* angle, short t){
	
	int line_num;
	short p1;
	int div;
	char aazw_str[10];
	report = 1;
	//if(t%25!=0){
	//	return 1;
	//} //降低print频率 现在转移到主函数中
	if(LCD_en == 0){
		// HC08:
		//p1 = aacw[2] - (short)g_value; //aacz
		//sprintf(aazw_str, "%d", p1);
		//HC08_SendString(aazw_str);
		//xUSART.USART1RecivedFlag=0; 
		return (aacw[2] - (short)g_value); 
		
	}
	else if(LCD_en != 1){
		return (aacw[2] - (short)g_value);
	}
	
	if(report){
		// aac:
		line_num = 100;
		div = 60;
		//////////
		LCD_ShowString(10,line_num,200,16,16,"AAC");
		p1 = aac[0]; //aacx
				if(p1<0)
				{
					LCD_ShowChar(30+0,line_num,'-',12,0);		//显示负号
					p1=-p1;		//转为正数
				}else LCD_ShowChar(30+0,line_num,' ',12,0);		//去掉负号 
				LCD_ShowNum(30+0+8,line_num,p1,5,12);		//显示整数部分	 
		//////////
		p1 = aac[1]; //aacy
				if(p1<0)
				{
					LCD_ShowChar(30+div,line_num,'-',12,0);		//显示负号
					p1=-p1;		//转为正数
				}else LCD_ShowChar(30+div,line_num,' ',12,0);		//去掉负号 
				LCD_ShowNum(30+div+8,line_num,p1,5,12);		//显示整数部分	
		/////////
		//////////
		p1 = aac[2]; //aacz
				if(p1<0)
				{
					LCD_ShowChar(30+div*2,line_num,'-',12,0);		//显示负号
					p1=-p1;		//转为正数
				}else LCD_ShowChar(30+div*2,line_num,' ',12,0);		//去掉负号 
				LCD_ShowNum(30+div*2+8,line_num,p1,5,12);		//显示整数部分	
		/////////
		
		// aac_world:
		line_num = 120;
		div = 60;
		//////////
		LCD_ShowString(10,line_num,200,16,16,"AAC");
		p1 = aacw[0]; //aacx
				if(p1<0)
				{
					LCD_ShowChar(30+0,line_num,'-',12,0);		//显示负号
					p1=-p1;		//转为正数
				}else LCD_ShowChar(30+0,line_num,' ',12,0);		//去掉负号 
				LCD_ShowNum(30+0+8,line_num,p1,5,12);		//显示整数部分	 
		//////////
		p1 = aacw[1]; //aacy
				if(p1<0)
				{
					LCD_ShowChar(30+div,line_num,'-',12,0);		//显示负号
					p1=-p1;		//转为正数
				}else LCD_ShowChar(30+div,line_num,' ',12,0);		//去掉负号 
				LCD_ShowNum(30+div+8,line_num,p1,5,12);		//显示整数部分	
		/////////
		//////////
		p1 = aacw[2] - (short)g_value; //aacz
				if(p1<0)
				{
					LCD_ShowChar(30+div*2,line_num,'-',12,0);		//显示负号
					p1=-p1;		//转为正数
				}else LCD_ShowChar(30+div*2,line_num,' ',12,0);		//去掉负号 
				LCD_ShowNum(30+div*2+8,line_num,p1,5,12);		//显示整数部分	
		/////////
				
		// gyro:
			/*
		line_num = 120;
		//////////
		p1 = gyro[0]; //gyrox
				if(p1<0)
				{
					LCD_ShowChar(30+0,line_num,'-',12,0);		//显示负号
					p1=-p1;		//转为正数
				}else LCD_ShowChar(30+0,line_num,' ',12,0);		//去掉负号 
				LCD_ShowNum(30+0+8,line_num,p1,5,12);		//显示整数部分	 
		//////////
		p1 = gyro[1]; //gyroy
				if(p1<0)
				{
					LCD_ShowChar(30+div,line_num,'-',12,0);		//显示负号
					p1=-p1;		//转为正数
				}else LCD_ShowChar(30+div,line_num,' ',12,0);		//去掉负号 
				LCD_ShowNum(30+div+8,line_num,p1,5,12);		//显示整数部分	
		/////////
		//////////
		p1 = gyro[2]; //gyroz
				if(p1<0)
				{
					LCD_ShowChar(30+div*2,line_num,'-',12,0);		//显示负号
					p1=-p1;		//转为正数
				}else LCD_ShowChar(30+div*2,line_num,' ',12,0);		//去掉负号 
				LCD_ShowNum(30+div*2+8,line_num,p1,5,12);		//显示整数部分	
		/////////
		*/
				
		// angle:
		line_num = 160;
		//////////
		p1 = (short)(angle[0]); //
				if(p1<0)
				{
					LCD_ShowChar(30+0,line_num,'-',16,0);		//显示负号
					p1=-p1;		//转为正数
				}else LCD_ShowChar(30+0,line_num,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+0+8,line_num,p1,5,16);		//显示整数部分	 
		//////////
		p1 = (short)(angle[1]); //gyroy
				if(p1<0)
				{
					LCD_ShowChar(30+32,line_num+20,'-',16,0);		//显示负号
					p1=-p1;		//转为正数
				}else LCD_ShowChar(30+32,line_num+20,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+32+8,line_num+20,p1,5,16);		//显示整数部分	
		/////////
		//////////
		p1 = (short)(angle[2]); //gyroz
				if(p1<0)
				{
					LCD_ShowChar(30+32*2,line_num+40,'-',16,0);		//显示负号
					p1=-p1;		//转为正数
				}else LCD_ShowChar(30+32*2,line_num+40,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+32*2+8,line_num+40,p1,5,16);		//显示整数部分	
		/////////
	}
	return (aacw[2] - (short)g_value);
}

float MPU_wRA(int t,float* wRA){
//		float pitch,roll,yaw;
//		float pitch_a,roll_a,yaw_a;

		float angle[3]; // not used
		float aac_average[3];
		float aac_w[3];
	  float f[9]; // not used
		long aac[3];
	  long w[3]; // not used
		short u[3]; // not used
		int i;
		aac_average[0] = 0.0;
		aac_average[1] = 0.0;
		aac_average[2] = 0.0;
		
		for(i=0;i<=t;i++){
			if(MPU6050_Report(1,aac,f,w,u,angle) != 0 ){
				i--;
				continue;
			}
			//MPU6050_Report(1, aac, wRA, aac_world, gyro, angle);
			aac_average[0] = (float)(aac_average[0] + (float)aac[0]);
			aac_average[1] = (float)(aac_average[1] + (float)aac[1]);
			aac_average[2] = (float)(aac_average[2] + (float)aac[2]);
			
		}
		aac_average[0] = (aac_average[0])/((float)(t+1));
		aac_average[1] = (aac_average[1])/((float)(t+1));
		aac_average[2] = (aac_average[2])/((float)(t+1));		
		GetwRA(wRA,aac_average);
		Matrix_33_31(aac_w,wRA,aac_average);
		g_value = aac_w[2];
		return( aac_w[2] );
}
