#include "main.h"

int main(){
	uint16_t buffA[BUFFER_SIZE];
	uint16_t buffB[BUFFER_SIZE];
	initTable(buffA,BUFFER_SIZE,0xAAFFU);
	initTable(buffB,BUFFER_SIZE,0x5577U);
	startup(buffA,BUFFER_SIZE);
	while(1){}
}


