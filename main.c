#include "main.h"
int main(){
	/*Tables initialisation*/
	//uint8_t buffA[BUFFER_SIZE_X];
	//uint8_t buffB[BUFFER_SIZE_X];
	//vgaBuffA = buffA;
	//vgaBuffB = buffB;
	
	/* Create basic frame */
	VGACreateFrame(vgaScreenBuff);
	VGADrawCircle(vgaScreenBuff, 8*(48>>1),150,64,0);
	//VGADrawRect(vgaScreenBuff,posX,posY-1,strlen(message)*8+1,0);
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

	}
}
