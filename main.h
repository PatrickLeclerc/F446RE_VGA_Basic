#ifndef MAIN_H
#define MAIN_H
#include "drivers.h"

/*////////////////Defines////////////////*/
#define BUFFER_SIZE (uint32_t) 24U

/*////////////////Variables////////////////*/
extern uint16_t* vgaBuffA;
extern uint16_t* vgaBuffB;
extern uint16_t*  vgaBuffNext;

/*////////////////Functions////////////////*/
/*Declarations*/
void TIM2_IRQHandler(void);
void initTable(uint16_t* table, uint32_t tableSize, uint16_t value);
void updateBuffer(uint16_t* table, uint32_t tableSize, uint32_t line);
/*Definitions*/
void updateBuffer(uint16_t* table, uint32_t tableSize, uint32_t line){
	initTable(table,tableSize,0);
	switch(line-2){
		case 0:
			table[2] |= 0x22;
			table[3] |= 0x3E3E;
			table[4] |= 0x3C00;
			break;
		case 1:
			table[2] |= 0x22;
			table[3] |= 0x2020;
			table[4] |= 0x2200;
			break;
		case 2:
			table[2] |= 0x22;
			table[3] |= 0x2020;
			table[4] |= 0x2200;
			break;
		case 3:
			table[2] |= 0x2A;
			table[3] |= 0x3C3C;
			table[4] |= 0x2200;
			break;
		case 4:
			table[2] |= 0x2A;
			table[3] |= 0x2020;
			table[4] |= 0x2200;
			break;
		case 5:
			table[2] |= 0x2A;
			table[3] |= 0x2020;
			table[4] |= 0x2200;
			break;
		case 6:
			table[2] |= 0x14;
			table[3] |= 0x3E3E;
			table[4] |= 0x3C00;
			break;
		default:
			break;
	}
}

void initTable(uint16_t* table, uint32_t tableSize, uint16_t value){
	table[0]=0x5550U;
	for(int i=1;i<(int)tableSize-1;i++){
		table[i]=value;
	}
	table[tableSize-1]=0x0555U;
}

#endif
