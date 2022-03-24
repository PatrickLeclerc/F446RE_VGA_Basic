#include "main.h"
uint16_t* vgaBuffA;
uint16_t* vgaBuffB;
uint16_t* vgaBuffNext;	/*for tim2 interupt*/
int main(){
	/*Tables initialisation*/
	uint16_t buffA[BUFFER_SIZE];
	uint16_t buffB[BUFFER_SIZE];
	vgaBuffA = buffA;
	vgaBuffB = buffB;
	initTable(buffA,BUFFER_SIZE,0);
	initTable(buffB,BUFFER_SIZE,0);
	/*Stm32 initialisation*/
	startup(BUFFER_SIZE);
	while(1){}
}
