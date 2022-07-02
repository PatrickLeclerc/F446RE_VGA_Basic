#include "main.h"
int main(){
	/* Create basic frame */
	VGACreateFrame(vgaScreenBuff);
	VGACreateFrame(vgaScreenBuff2);
	VGADrawCircle(vgaScreenBuff,48<<2,300>>1,64,0);
	VGADrawRect(vgaScreenBuff2,48<<2,300>>1,64,0);
	/*Stm32 initialisation*/
	startup(BUFFER_SIZE_X);
	while(1){
		if(uartRxFlag){
			/* Clear flag */
			uartRxFlag = 0;
			
			/* Print data on screen */
			VGACreateFrame(vgaScreenBuff);
			//VGADrawCircle(vgaScreenBuff, 29*8,150,64,1);
			int posX = 15;
			int posY = 5;
			for(int i=0;i<strlen(uartBuff);i++){
				VGAPutChar(vgaScreenBuff, posX+8*i,posY,uartBuff[i]);
			}
			/* Clear the buffer */
			memset(uartBuff,0,UARTBUFFSIZE);
		}
		if(vgaFlag){
			vgaFlag = 0;
			
		}
	}
}
