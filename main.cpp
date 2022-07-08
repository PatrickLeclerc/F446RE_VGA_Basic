#include "main.h"
#include <string>
#include <math.h>

int main(){
	const float pi = 3.14159265359;
	/* Create basic frame */
	vgaBuffNext = vgaScreenBuff;
	VGACreateFrame(vgaScreenBuff);
	VGACreateFrame(vgaScreenBuff2);
	
	/*Stm32 initialisation*/
	startup(BUFFER_SIZE_X);
	
	
	
	while(1){
		if(vgaFlag){
			/* Basics */
			vgaFlag = 0;
			VGABlankScreen(vgaNextScreenBuff);
			VGACreateFrame(vgaNextScreenBuff);
			
			#if 1 //Losange
			int N = 5;
			Pt_t *pts = new Pt_t [N];
			
			int a = 10;
			int b = 10;
			
			pts[0].x = BUFFER_BITSIZE_X/2;
			pts[0].y = BUFFER_SIZE_Y/2 + b;
			pts[1].x = BUFFER_BITSIZE_X/2+a;
			pts[1].y = BUFFER_SIZE_Y/2;
			pts[2].x = BUFFER_BITSIZE_X/2;
			pts[2].y = BUFFER_SIZE_Y/2-b;
			pts[3].x = BUFFER_BITSIZE_X/2-a;
			pts[3].y = BUFFER_SIZE_Y/2;
			pts[4].x = BUFFER_BITSIZE_X/2;
			pts[4].y = BUFFER_SIZE_Y/2 + b;
			drawShape(pts,N);
			delete[] pts;
			
			#endif
			#if 1 //Square wave
			
			static float t = 0.0;
			
			for(int x = 0; x<BUFFER_BITSIZE_X ;x++)
			{	
				/* Variables */
				const static float f = 1.0;
				const static float f0 = 2*pi*f/float(BUFFER_BITSIZE_X);
				static float A =  75*4.0/pi;
				
				static Pt_t pt1 = {};
				pt1.x = x;
				pt1.y = 150.0;

				
				for(float n = 1.0;n<51.0;n+=2.0){
					
					pt1.y += A / n * sin(float(n*f0*(x-t-1)));
				}			
				VGApx(vgaNextScreenBuff,pt1.x,pt1.y);
			}
			t+=0.75;
			#endif
			
			#if 1 //Text
			std::string message = "GIT::F446RE_VGA_Basic -- CPP";
			for(unsigned int i=0;i<message.length();i++){
				VGAPutChar(vgaNextScreenBuff, (BUFFER_BITSIZE_X>>1)+i*8U-message.length()*4,0,message[i]);
			}
			VGADrawRect(vgaNextScreenBuff,(BUFFER_BITSIZE_X>>1)-message.length()*4,0,message.length()*8, 8,0);
			#endif
			#if 1 //Spinning stuff
			static const double R = 64;
			static double t1 = 0;
			VGADrawCircle(vgaNextScreenBuff,BUFFER_BITSIZE_X>>2,BUFFER_SIZE_Y>>1,7,1);
			for(double i=0.0;i<5.0;i+=0.5){
				double x = 0.5*R*sin(t1+i) + double(BUFFER_BITSIZE_X>>1);
				double y = R*cos(t1+i) + double(BUFFER_SIZE_Y>>1);
				VGADrawLine(vgaNextScreenBuff,int(x),int(y),int(x)+50,int(y));
				x = 0.5*R*sin(-4.0*t1+i/2) + double(BUFFER_BITSIZE_X>>2);
				y = 0.1*0.5*R*cos(-4.0*t1+i/2) + double(BUFFER_SIZE_Y>>1);
				VGApx(vgaNextScreenBuff,int(x),int(y));
			}
			t1+=0.01;
			#endif
			#if 1 //Balls
			/* Space */
			static const int r = 16; 
			static int x = 64;
			static int y = 64;
			
			/* T */
			static const int dT=1;
			
			/* X */
			static int vX = 3;
			x += dT*vX;
			if( ( x>(BUFFER_BITSIZE_X-r) ) || (x<=r) ) vX *= -1;
			
			/*Y*/
			static int vY = 11;
			vY+=2;
			y += dT*vY;
			if( y>(BUFFER_SIZE_Y-r) ) {
				vY *= -1;
				y=BUFFER_SIZE_Y-r;
			}
			else if(y<=r){
				vY *= -1;
				y=0;
			}
			
			/* Place circle */
			VGADrawCircle(vgaNextScreenBuff,x,y,r,1);
			VGADrawCircle(vgaNextScreenBuff,BUFFER_BITSIZE_X-x-1,y,r,1);
			#endif


		}
	}
}