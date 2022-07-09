#include "main.h"
#include <string>


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
			#if 1//Time
			drawTime(200,150);
			#endif
			#if 0 //RotatingClock
			VGADrawCircle(vgaNextScreenBuff,BUFFER_BITSIZE_X/2,BUFFER_SIZE_Y/2,8,0);
			VGADrawCircle(vgaNextScreenBuff,BUFFER_BITSIZE_X/2,BUFFER_SIZE_Y/2,4,1);
			const int N = 5;
			Pt_t pts[N] = {} ;
			static float tFast = 0;
			static float tSlow = 0;
			static float tVerySlow = 0;
			int a = 0.0;
			int b = 0.0;
			float x = 0.0;
			float y = 0.0;
			
			/* Hour pin */
			{
			a = 2.0;	
			b = 125.0;	
			pts[0].x = 0;
			pts[0].y = -b;
			pts[1].x = a;
			pts[1].y = 0;
			pts[2].x = 0;	
			pts[2].y = b/2;
			pts[3].x = -a;
			pts[3].y = 	0;
			pts[4].x = pts[0].x;
			pts[4].y = pts[0].y;
			}	
			/* rotate losange */
			for(int i = 0;i<N;i++){
				
				/* Apply rotation */
				Pt_t ptRotated = applyRotation(pts[i],2*pi*tVerySlow);
				
				/* Offset */
				ptRotated.x += float(BUFFER_BITSIZE_X)/2.0f;
				ptRotated.y += float(BUFFER_SIZE_Y)/2.0f;
				
				/* Assign new point coordinates */
				pts[i] = ptRotated;
			}
			
			drawShape(pts,N);
			/* Minute pin */
			{
			a = 8.0;	
			b = 100.0;	
			pts[0].x = 0;
			pts[0].y = -b;
			pts[1].x = a;
			pts[1].y = 0;
			pts[2].x = 0;	
			pts[2].y = b/2;
			pts[3].x = -a;
			pts[3].y = 	0;
			pts[4].x = pts[0].x;
			pts[4].y = pts[0].y;
			}	
			/* rotate losange */
			for(int i = 0;i<N;i++){
				
				/* Apply rotation */
				Pt_t ptRotated = applyRotation(pts[i],2*pi*tSlow);
				
				/* Offset */
				ptRotated.x += float(BUFFER_BITSIZE_X)/2.0f;
				ptRotated.y += float(BUFFER_SIZE_Y)/2.0f;
				
				/* Assign new point coordinates */
				pts[i] = ptRotated;
			}
			
			drawShape(pts,N);
			/* Second pin */
			{
			a = 5.0;
			b = 50.0;
			pts[0].x = 0;
			pts[0].y = -b;
			pts[1].x = a;
			pts[1].y = 0;
			pts[2].x = 0;	
			pts[2].y = b/2;
			pts[3].x = -a;
			pts[3].y = 	0;
			pts[4].x = pts[0].x;
			pts[4].y = pts[0].y;
			}
			/* rotate losange */
			for(int i = 0;i<N;i++){
				
				/* Apply rotation */
				Pt_t ptRotated = applyRotation(pts[i],2*pi*tFast);
				
				/* Offset */
				ptRotated.x += float(BUFFER_BITSIZE_X)/2.0f;
				ptRotated.y += float(BUFFER_SIZE_Y)/2.0f;
				
				/* Assign new point coordinates */
				pts[i] = ptRotated;
			}
			drawShape(pts,N);
			
			/*draw clock frame*/
			VGADrawCircle(vgaNextScreenBuff,BUFFER_BITSIZE_X/2,BUFFER_SIZE_Y/2,125,0);
			
			/* Increment time */
			const static float tVerySlowpp = 1.0f/(56.0f*60.0f*60.0f);
			const static float tSlowpp = 1.0f/(56.0f*60.0f);
			const static float tFastpp = 1.0/56.0f;
			tVerySlow -= tVerySlowpp;
			tSlow -= tSlowpp;
			tFast -= tFastpp;
			#endif
			#if 0 //Square wave
			
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
			#if 0 //Spinning stuff
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
			#if 0 //Balls
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
