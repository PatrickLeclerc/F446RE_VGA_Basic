#include "main.h"
int main(){
	/* Create basic frame */
	vgaBuffNext = vgaScreenBuff;
	VGACreateFrame(vgaScreenBuff);
	VGACreateFrame(vgaScreenBuff2);
	//VGADrawCircle(vgaScreenBuff,48<<2,300>>1,7,1);
	//VGADrawCircle(vgaScreenBuff2,48<<2,300>>1,7,1);
	//VGADrawRect(vgaScreenBuff2,(48<<2-48<<1),300>>1,64<<1,64,0);
	/*Stm32 initialisation*/
	startup(BUFFER_SIZE_X);
	while(1){
		if(uartRxFlag){
			/* Clear flag */
			uartRxFlag = 0;
			/* Print data on screen */
			VGACreateFrame(vgaNextScreenBuff);
			//VGADrawCircle(vgaScreenBuff, 29*8,150,64,1);
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
			static const int r = 16; 
			vgaFlag = 0;
			//VGACreateFrame(vgaNextScreenBuff);
			
			/* Space */
			//static int dirX = 0;
			//static int dirY = 0;
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
