#include "main.h"
uint8_t* vgaBuffA;
uint8_t* vgaBuffB;
uint8_t* vgaBuffNext;	/*for tim2 interupt*/
int main(){
	/*Tables initialisation*/
	uint8_t buffA[BUFFER_SIZE];
	uint8_t buffB[BUFFER_SIZE];
	vgaBuffA = buffA;
	vgaBuffB = buffB;
	initTable(buffA,BUFFER_SIZE,0);
	initTable(buffB,BUFFER_SIZE,0);
	/*Stm32 initialisation*/
	startup(BUFFER_SIZE);
	while(1){}
}
