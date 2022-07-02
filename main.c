#include "main.h"
uint8_t* vgaBuffNext;	/*for tim2 interupt*/
int main(){
	/*Tables initialisation*/
	//uint8_t buffA[BUFFER_SIZE_X];
	//uint8_t buffB[BUFFER_SIZE_X];
	//vgaBuffA = buffA;
	//vgaBuffB = buffB;
	
	/* Create basic frame */
	VGACreateFrame(vgaScreenBuff);
	VGADrawCircle(vgaScreenBuff, 29*8,150,64,1);
	
	int posX = 15;
	int posY = 5;
	char message[] = "Hello World!";
	for(int i=0;i<strlen(message);i++){
		VGAPutChar(vgaScreenBuff, posX+8*i,posY,message[i]);
	}
	VGADrawRect(vgaScreenBuff,posX,posY-1,strlen(message)*8+1,0);
	/*Stm32 initialisation*/
	startup(BUFFER_SIZE_X);
	printUart2("Hello\n\r");
	while(1){
		if(uartRxFlag){
			/* Clear flag */
			uartRxFlag = 0;
			
			/* Use the data */
			printUart2((char*)uartBuff);
			
			/* Clear the buffer */
			memset(uartBuff,0,UARTBUFFSIZE);
		}

	}
}
