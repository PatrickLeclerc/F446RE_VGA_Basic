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
void initTable(uint16_t* table, uint16_t tableSize, uint16_t value);

/*Definitions*/


void initTable(uint16_t* table, uint16_t tableSize, uint16_t value){
	table[0]=0x5550U;
	for(int i=1;i<(int)tableSize-1;i++){
		table[i]=value;
	}
	table[tableSize-1]=0x0555U;
}

#endif
