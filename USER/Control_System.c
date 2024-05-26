#include "Control_System.h"

void HC08_Press5(){
		u16 x;
		delay_us(2000);
		EN_V=0;
		DIR_V=0;
		
		while(1){
			for( x= 0; x<100;x++)
			{
				//
				delay_us(2000);
				PWM_V= 1;
				delay_us(steer_speed_choice()); //set the delay value to change motor speed
				PWM_V= 0;
				delay_us(steer_speed_choice());
				
			}
			if(xUSART.USART2RecivedFlag==1){
				HC08_SendString((char*)xUSART.USART2RecivedBuffer);
				xUSART.USART2RecivedFlag=0;      
				if(xUSART.USART2RecivedBuffer[0]=='5'){
					HC08_SendString("\nPress 5 END\n");
					xUSART.USART1RecivedFlag=0;
					EN_V=1;
					return;
				}
			}
		}
		

}

void HC08_Press6(){
		u16 x;
		delay_us(2000);
		EN_V=0;
		DIR_V=1;
		while(1){
			for( x= 0; x<100;x++)
			{
				delay_us(2000);
				PWM_V= 1;
				delay_us(steer_speed_choice()); //set the delay value to change motor speed
				PWM_V= 0;
				delay_us(steer_speed_choice());
				
			}
			if(xUSART.USART2RecivedFlag==1){
				HC08_SendString((char*)xUSART.USART2RecivedBuffer);
				xUSART.USART2RecivedFlag=0;      
				if(xUSART.USART2RecivedBuffer[0]=='6'){
					HC08_SendString("\nPress 6 END\n");
					xUSART.USART1RecivedFlag=0;
					EN_V=1;
					return;
				}
			}
		}
		

}


void steer_Control(float range,float speed){
		int x;
		EN_V = 0;
		//delay_us(2000);
		if( range>=0 ){
			DIR_V = 1; //6
		}
		else{ //5
			DIR_V = 0;
			range = -range;
		}
		
		//if(range >= 800.0){
		//	range = 800.0;
		//}
		for( x= 0; x<=range ;x++)
		{

				PWM_V= 1;
				delay_us(((int)60000/speed));
				PWM_V= 0;
				delay_us(((int)60000/speed));
			}
		EN_V = 1;
}

float Sigmoid_reciprocal(float L, float k_reciprocal, float x0, float x){
	float denominator;
	denominator = 1.0 + exp(-(x-x0)/k_reciprocal);
	return(L/denominator-L/2.0);
	

}

float Sigmoid(float L, float k, float x0, float x){
		float denominator;
		denominator = 1.0 + exp(-(x-x0)*k);
		return(L/denominator-L/2.0);
}

void Acc_Control(float La, float ka, float Lv, float kv, float range_max, int aazw,int ADC_delta){
	//exp();
	float L;
	float k_reciprocal;
	float x0;
	float range;
	//L = 800.0;
	//x0 = 0.0;
	//k_reciprocal = 200.0;
	range = Sigmoid(La,ka,0.0,aazw)+Sigmoid(Lv,kv,0.0,ADC_delta);
	if(range >= range_max){
		range = range_max;
	}
	if(range <= -range_max){
		range = -range_max;
	}
	steer_Control(range,30.0);
	
}



