#include "main.h"

int main(){
	uint16_t buffA[BUFFER_SIZE];
	uint16_t buffB[BUFFER_SIZE];
	initTable(buffA,BUFFER_SIZE,0xAAFFU);
	initTable(buffB,BUFFER_SIZE,0x5577U);
	startup(buffA,BUFFER_SIZE);
	while(1){}
}

void TIM2_IRQHandler(){
	static uint16_t counter = 0U;
	if(TIM2->SR & TIM_SR_UIF){
		//ClearTC & THC flags from DMA chxx streamxx
		//Set MOAR
		//Set NDTR??
		DMA1_Stream4->CR |= DMA_SxCR_EN;
		counter++;
		if(counter==625U){
			counter=0U;
		}
		//Reset UIF
		TIM2->SR &= ~TIM_SR_UIF;
	}
}

