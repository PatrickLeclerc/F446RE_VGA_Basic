#include "main.h"
int main(){
	/* Create basic frame */
	vgaBuffNext = vgaScreenBuff;
	VGACreateFrame(vgaScreenBuff);
	VGACreateFrame(vgaScreenBuff2);
	
	/*Stm32 initialisation*/
	startup(BUFFER_SIZE_X);
	
	while(1){
		if(uartRxFlag){
			/* Clear flag */
			uartRxFlag = 0;
			
			/* Print data on screen */
			VGACreateFrame(vgaNextScreenBuff);
			int posX = 0;
			int posY = 5;
			for(int i=0;i<strlen(uartBuff);i++){
				VGAPutChar(vgaScreenBuff, posX+8*i,posY,uartBuff[i]);
				VGAPutChar(vgaScreenBuff2, posX+8*i,posY,uartBuff[i]);
			}
			
			/* Clear the buffer */
			memset(uartBuff,0,UARTBUFFSIZE);
		}
		if(vgaFlag){
			/* Basics */
			VGACreateFrame(vgaNextScreenBuff);
			VGADrawLine(vgaNextScreenBuff,64,64,0,0);
			VGADrawLine(vgaNextScreenBuff,0,64,64,0);
			VGADrawLine(vgaNextScreenBuff,0,64,64,64);
			VGADrawLine(vgaNextScreenBuff,64,0,64,64);
			static const int r = 16; 
			vgaFlag = 0;
			
			/* Space */
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
			
			/*Write text*/
			char message[] = "Bouncing balls";
			for(int i=0;i<strlen(message);i++){
				VGAPutChar(vgaNextScreenBuff, 191+8*i,150,message[i]);
			}

		}
	}
}
