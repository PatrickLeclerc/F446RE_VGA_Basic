#ifndef MAIN_H
#define MAIN_H
#include "drivers.h"

/*////////////////Defines////////////////*/
#define BUFFER_SIZE (uint32_t) 26U

/*////////////////Variables////////////////*/


/*////////////////Functions////////////////*/
/*Declarations*/
void TIM2_IRQHandler(void);
void initTable(uint16_t* table, uint16_t tableSize, uint16_t value);
/*Definitions*/


void initTable(uint16_t* table, uint16_t tableSize, uint16_t value){
	for(uint16_t i=0;i<tableSize;i++){
		table[i]=value;
	}
}
#endif
