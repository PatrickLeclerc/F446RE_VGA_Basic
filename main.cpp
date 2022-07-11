#include "main.h"
#include <string>
extern "C"{
#include "arm_math.h"
}

int main(){
	/* Init buffers for interrupts*/
	vgaBuffNextR = vgaScreenBuffR;
	vgaBuffNextG = vgaScreenBuffG;
	vgaBuffNextB = vgaScreenBuffB;
	/*Stm32 initialisation*/
	startup(BUFFER_SIZE_X);
	
	while(1){
		if(vgaFlag){
			/* Basics */
			vgaFlag = 0;
			#if 1 //Blank
			VGABlankScreen(vgaNextScreenBuffR);
			VGABlankScreen(vgaNextScreenBuffG);
			VGABlankScreen(vgaNextScreenBuffB);
			#endif
			#if 1 //Frame
			//VGACreateFrame(vgaNextScreenBuffR);
			VGACreateFrame(vgaNextScreenBuffG);
			VGACreateFrame(vgaNextScreenBuffB);
			#endif
			#if 1 //Time
			drawTime(BUFFER_BITSIZE_X-64,0,false);
			//VGADrawRect(vgaNextScreenBuffR,BUFFER_BITSIZE_X-66,0,64,8,0);
			VGADrawRect(vgaNextScreenBuffG,BUFFER_BITSIZE_X-66,0,64,8,0);
			VGADrawRect(vgaNextScreenBuffB,BUFFER_BITSIZE_X-66,0,64,8,0);
			#endif
			#if 1 //Program name
			std::string message = "F446RE_VGA_Basic";
			for(unsigned int i=0;i<message.length();i++){
				//VGAPutChar(vgaNextScreenBuffR, (BUFFER_BITSIZE_X>>1)+i*8U-message.length()*4,0,message[i]);
				VGAPutChar(vgaNextScreenBuffG, (BUFFER_BITSIZE_X>>1)+i*8U-message.length()*4,0,message[i]);
				VGAPutChar(vgaNextScreenBuffB, (BUFFER_BITSIZE_X>>1)+i*8U-message.length()*4,0,message[i]);
			}
			//VGADrawRect(vgaNextScreenBuffR,(BUFFER_BITSIZE_X>>1)-message.length()*4,0,message.length()*8, 8,0);
			VGADrawRect(vgaNextScreenBuffG,(BUFFER_BITSIZE_X>>1)-message.length()*4,0,message.length()*8, 8,0);
			VGADrawRect(vgaNextScreenBuffB,(BUFFER_BITSIZE_X>>1)-message.length()*4,0,message.length()*8, 8,0);
			#endif
			/* Movement */
			#if 1 //Maschine
			{
				float X = 8;
				float Y = 64;
				float L = 256;
				float H = 16;
				float D = 100;
				{//Sides
					Pt_t ptsL[] = {
						{X,Y},
						{X+32,Y+32},
						{X+32,Y+40},
						{X+D+32,Y+D+40},
						{X+D+32,Y+D+40+H},
						{X,Y+8+H},
						{X,Y}
					};
					drawShape(ptsL,sizeof(ptsL)/sizeof(Pt_t));
					Pt_t ptsR[] = {
						{ptsL[0].x+L,ptsL[0].y},
						{ptsL[1].x+L,ptsL[1].y},
						{ptsL[2].x+L,ptsL[2].y},
						{ptsL[3].x+L,ptsL[3].y},
						{ptsL[4].x+L,ptsL[4].y}
					};
					drawShape(ptsR,sizeof(ptsR)/sizeof(Pt_t));
					Pt_t ptsHorizontal[] = {
						ptsL[0],
						ptsR[0],
						ptsL[1],
						ptsR[1],
						ptsL[2],
						ptsR[2],
						ptsL[3],
						ptsR[3],
						ptsL[4],
						ptsR[4]
					};
					for(int i = 0; i <10 ; i+=2)
						drawShape(&ptsHorizontal[i],2);
					
					X += 128;
					Y += 48;
					for(int i = 0; i <4 ; i++){
						for(int j = 0; j<4 ; j++)
						{
							int padL = 40;
							Pt_t ptsPad[] = {
								{X+padL*i+padL/2*j,Y+padL/2*j},
								{X+padL*i+padL/2*j+16,Y+16+padL/2*j},
								{X+padL*i+padL/2*j+48,Y+16+padL/2*j},
								{X+padL*i+padL/2*j+32,Y+padL/2*j},
								{X+padL*i+padL/2*j,Y+padL/2*j}
							};
							drawShape(ptsPad,sizeof(ptsPad)/sizeof(Pt_t));
						}
					}
				}


			}
			#endif
			#if 1 //RGB Showoff
				int x = BUFFER_BITSIZE_X/3;
				int y = BUFFER_SIZE_Y/3;
				int r = 80;			
				VGADrawCircle(vgaNextScreenBuffR,x,y,r,1);
				VGADrawCircle(vgaNextScreenBuffG,x*1.5,y*2,r,1);
				VGADrawCircle(vgaNextScreenBuffB,x*2,y,r,1);
			
				//VGADrawCircle(vgaNextScreenBuffR,BUFFER_BITSIZE_X/2,BUFFER_SIZE_Y/2,r,1);
				//VGADrawCircle(vgaNextScreenBuffG,BUFFER_BITSIZE_X/2,BUFFER_SIZE_Y/2,r,1);
				//VGADrawCircle(vgaNextScreenBuffB,BUFFER_BITSIZE_X/2,BUFFER_SIZE_Y/2,r,1);
			#endif
			#if 0 //Ellipse
			{
				int RA = 32;
				int RB = RA;
				drawEllipse(BUFFER_BITSIZE_X/2,BUFFER_SIZE_Y/2,RA,RB/4);
				drawEllipse(BUFFER_BITSIZE_X/2,BUFFER_SIZE_Y/2,RA/2,RB/8);
				drawEllipse(BUFFER_BITSIZE_X/2,BUFFER_SIZE_Y/2,RA/4,RB/16);
			}
			#endif
			#if 0 //Small clock		
			drawSmallClock(BUFFER_BITSIZE_X/2,BUFFER_SIZE_Y/4);
			//for(int y = 0; y<BUFFER_SIZE_Y/48;y+=2)
			//	for(int x = 0; x<BUFFER_BITSIZE_X/48;x+=2)
			//		drawSmallClock(24+x*48,24+y*48);
			#endif

			#if 0 //Rotating clock
			VGADrawCircle(vgaNextScreenBuffG,BUFFER_BITSIZE_X/2,BUFFER_SIZE_Y/2,8,0);
			VGADrawCircle(vgaNextScreenBuffG,BUFFER_BITSIZE_X/2,BUFFER_SIZE_Y/2,4,1);
			const int N = 5;
			Pt_t pts[N] = {} ;
			static float tFast = 0;
			static float tSlow = 0;
			static float tVerySlow = 0;
			int a = 0.0;
			int b = 0.0;
			float x = 0.0;
			float y = 0.0;
			#if 0
			/* Hour pin */
			{
			a = -4.0;	
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
			#endif
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
			VGADrawCircle(vgaNextScreenBuffG,BUFFER_BITSIZE_X/2,BUFFER_SIZE_Y/2,125,0);
			
			/* Increment time */
			const static float tVerySlowpp = 1.0f/(56.0f*60.0f*60.0f*24.0f);
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

				
				for(float n = 1.0;n<25.0;n+=2.0){
					
					pt1.y += A / n * arm_cos_f32(float(n*f0*(x-t-1)));
				}			
				VGApx(vgaNextScreenBuffB,pt1.x,pt1.y,INV);
			}
			t+=0.75;
			#endif
			
			#if 0 //Spinning stuff
			static const double R = 64;
			static double t1 = 0;
			VGADrawCircle(vgaNextScreenBuffR,BUFFER_BITSIZE_X>>2,BUFFER_SIZE_Y>>1,7,1);
			for(double i=0.0;i<5.0;i+=0.5){
				double x = 0.5*R*sin(t1+i) + double(BUFFER_BITSIZE_X>>1);
				double y = R*cos(t1+i) + double(BUFFER_SIZE_Y>>1);
				VGADrawLine(vgaNextScreenBuffR,int(x),int(y),int(x)+50,int(y));
				x = 0.5*R*sin(-4.0*t1+i/2) + double(BUFFER_BITSIZE_X>>2);
				y = 0.1*0.5*R*cos(-4.0*t1+i/2) + double(BUFFER_SIZE_Y>>1);
				VGApx(vgaNextScreenBuffR,int(x),int(y));
			}
			t1+=0.01;
			#endif
			#if 0 //Balls
			{
				/* Space */
				static const int rBalls = 16; 
				static int x = 64;
				static int y = 64;
				
				/* T */
				static const int dT=1;
				
				/* X */
				static int vX = 3;
				x += dT*vX;
				if( ( x>(BUFFER_BITSIZE_X-rBalls) ) || (x<=rBalls) ) vX *= -1;
				
				/*Y*/
				static int vY = 11;
				vY+=2;
				y += dT*vY;
				if( y>(BUFFER_SIZE_Y-rBalls) ) {
					vY *= -1;
					y=BUFFER_SIZE_Y-rBalls;
				}
				else if(y<=rBalls){
					vY *= -1;
					y=0;
				}
				
				/* Place circle */
				VGADrawCircle(vgaNextScreenBuffB,x,y,rBalls,1);
				VGADrawCircle(vgaNextScreenBuffB,BUFFER_BITSIZE_X-x-1,y,rBalls,1);
			}
			#endif


		}
	}
}
