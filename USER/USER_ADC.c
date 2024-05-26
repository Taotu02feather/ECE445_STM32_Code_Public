#include "USER_ADC.h"


u16 ADC_Get(){
		u16 adcx;
		char ADC_GET_FLAG[10];
		adcx=Get_Adc_Average(ADC_Channel_1,10);
		
		sprintf(ADC_GET_FLAG,"FLAG%d \n",adcx);
		//HC08_SendString(ADC_GET_FLAG);
		//xUSART.USART1RecivedFlag=0;  
		return adcx;
}

int ADC_Print(u16 num){
	float temp;
	u16 adcx;
	int x = 30;
	int y = 80;
	char ADC_HC08_str[15];
	adcx = num;
	if( LCD_en == 1 ){
		LCD_ShowxNum(x,y,adcx,4,16,0);//显示ADC的值
	}
	else if(LCD_en == 0){
		sprintf(ADC_HC08_str,"ADC %d \n",num);

		HC08_SendString(ADC_HC08_str);
		xUSART.USART1RecivedFlag=0;  
		return (int)adcx;
	}
	else{
		return 2;
	}
	temp=(float)adcx*(3.3/4096);
	adcx=temp;
	LCD_ShowxNum(x,y+20,adcx,1,16,0);//显示电压值
	temp-=adcx;
	temp*=1000;
	LCD_ShowxNum(x+(172-156),y+40,temp,3,16,0X80);
	return (int)adcx;
	
}