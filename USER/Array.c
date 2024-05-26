#include "Array.h"

void delay_n1000ms(int n){
	int i;
	for(i=1;i<=n;i++){
		delay_ms(1000);
		
	}
}

int PWM_Duty_Set(int f,float Duty){
	float d;
	d = 1.0-(Duty/100.0);
	d = d*(float)(f);
	d = d - 1;
	return (int)d;

}

int GetIndex(int x,int y){

	return(y+3*x-4);
	
}

void Matrix_33_33(float* AB, float* A, float* B){
	/* Comment:
	A should be: float[9]
	B should be: float[9]
	AB should be float[9]
	the format is:
	1 2 3
	 4 5 6
	 7 8 9
	
	*/
	int x;
	int y;
	int k;
	float xy_r;
	for(x=1;x<=3;x++){
		for(y=1;y<=3;y++){
			
			xy_r = 0.0;
			for(k=1;k<=3;k++){
				
				xy_r = xy_r + A[GetIndex(x,k)] * B[GetIndex(k,y)];
				AB[GetIndex(x,y)] = xy_r;
			}
		}
	}
	
}

void Matrix_33_31(float* Av, float* A, float* v){
	/* Comment:
	A should be: float[9]
	v should be: float[3]
	A format:
	{
	1 2 3
	4 5 6
	7 8 9
	}
	v and Av format:
	{1
	 2
	 3}
	
	
	*/
	int x;
	int y;
	int k;
	float xy_r;
	for(x=1;x<=3;x++){
		for(y=1;y<=1;y++){
			
			xy_r = 0.0;
			for(k=1;k<=3;k++){
				
				xy_r = xy_r + A[GetIndex(x,k)] * v[(k+y-2)];
				
			}
			Av[(x+y-2)] = xy_r;
		}
	}
}
double GetNorm(float* v){
	double r;
	
	r = (double)(v[0])*(double)(v[0]) 
		+ (double)(v[1])*(double)(v[1]) 
		+ (double)(v[2])*(double)(v[2]);
	
	return sqrt(r);
}
double GetNormxyz(float x, float y, float z){

	return sqrt(x*x+y*y+z*z);
	
}


void GetRx(float* Rx, float roll){
		float c,s;
		c = cos(roll/57.295779);
		s = sin(roll/57.295779);
		Rx[GetIndex(1,1)] = 1.0;
		Rx[GetIndex(1,2)] = 0.0;
		Rx[GetIndex(1,3)] = 0.0;
	
		Rx[GetIndex(2,1)] = 0.0;
		Rx[GetIndex(2,2)] = c;
		Rx[GetIndex(2,3)] = -s;
	
		Rx[GetIndex(3,1)] = 0.0;
		Rx[GetIndex(3,2)] = s;
		Rx[GetIndex(3,3)] = c;
}

void GetRy(float* Ry, float pitch){
		float c,s;
		c = cos(pitch/57.295779);
		s = sin(pitch/57.295779);
		Ry[GetIndex(1,1)] = c;
		Ry[GetIndex(1,2)] = 0.0;
		Ry[GetIndex(1,3)] = s;
	
		Ry[GetIndex(2,1)] = 0.0;
		Ry[GetIndex(2,2)] = 1.0;
		Ry[GetIndex(2,3)] = 0.0;
	
		Ry[GetIndex(3,1)] = -s;
		Ry[GetIndex(3,2)] = 0.0;
		Ry[GetIndex(3,3)] = c;
}

void GetRz(float* Rz, float yaw){
		float c,s;
		c = cos(yaw/57.295779);
		s = sin(yaw/57.295779);
		Rz[GetIndex(1,1)] = c;
		Rz[GetIndex(1,2)] = -s;
		Rz[GetIndex(1,3)] = 0.0;
	
		Rz[GetIndex(2,1)] = s;
		Rz[GetIndex(2,2)] = c;
		Rz[GetIndex(2,3)] = 0.0;
	
		Rz[GetIndex(3,1)] = 0.0;
		Rz[GetIndex(3,2)] = 0.0;
		Rz[GetIndex(3,3)] = 1.0;

}


void GetwRA(float* wRA, float* Aa){
	float x,y,z;
	double N1,N2,N3;
	x = (float)Aa[0];
	y = (float)Aa[1];
	z = (float)Aa[2];
	N1 = GetNormxyz(-y ,x, 0.0);
	N2 = GetNormxyz(x*z ,y*z, -x*x-y*y);
	N3 = GetNormxyz(x,y,z);
	
	wRA[GetIndex(1,1)] = -y/N1;
	wRA[GetIndex(2,1)] = x/N1;
	wRA[GetIndex(3,1)] = 0.0/N1;
	
	wRA[GetIndex(1,2)] = (x*z)/N2;
	wRA[GetIndex(2,2)] = (y*z)/N2;
	wRA[GetIndex(3,2)] = (-x*x-y*y)/N2;
	
	wRA[GetIndex(1,3)] = x/N3;
	wRA[GetIndex(2,3)] = y/N3;
	wRA[GetIndex(3,3)] = z/N3;
}

void GetARB(float* ARB, float* angle){
		float Rx[9];
		float Ry[9];
		float Rz[9];
		float RxRy[9];
		float RxRyRz[9];
		float pitch, roll, yaw;
		int i;
		pitch = angle[0];
		roll = angle[1];
		yaw = angle[2];
	
		GetRx(Rx, roll);
		GetRy(Ry, pitch);
		GetRz(Rz, yaw);
		
		Matrix_33_33(RxRy,Rx,Ry);
		Matrix_33_33(RxRyRz,RxRy,Rz);
		for(i=0;i<=8;i++){
			ARB[i] = RxRyRz[i];
		}
		
}