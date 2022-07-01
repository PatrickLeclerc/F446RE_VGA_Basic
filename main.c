#include "main.h"
uint8_t* vgaBuffNext;	/*for tim2 interupt*/
int main(){
	/*Tables initialisation*/
	uint8_t buffA[BUFFER_SIZE_X];
	uint8_t buffB[BUFFER_SIZE_X];
	vgaBuffA = buffA;
	vgaBuffB = buffB;
	
	/* Create basic frame */
	VGACreateFrame(vgaScreenBuff);
	VGADrawCircle(vgaScreenBuff, 29*8,150,64,0);
	VGADrawRect(vgaScreenBuff,29*8-32,150-32,64,0);
	/*Stm32 initialisation*/
	startup(BUFFER_SIZE_X);
	while(1){}
}
