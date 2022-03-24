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
	initTable(buffA,BUFFER_SIZE,0xAF53U);
	initTable(buffB,BUFFER_SIZE,0x1234U);
	/*Stm32 initialisation*/
	for(int i = 0 ; i<1000000; i++){}/*Delay*/
	startup(BUFFER_SIZE);
	while(1){}
}

void TIM2_IRQHandler(){
	static uint16_t line = 0U;
	if(TIM2->SR & TIM_SR_UIF){
		/*Handling DMA1_STREAM4_CH0 stuff*/
		DMA1->HIFCR |= DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4;
		DMA1_Stream4->M0AR = (uint32_t)vgaBuffNext;
		DMA1_Stream4->CR |= DMA_SxCR_EN;//Start
		/*Calculate next buffer once every two lines*/
		line++;
		if((line&1)==0){
			if(vgaBuffNext==vgaBuffA) vgaBuffNext = vgaBuffB;
			else vgaBuffNext = vgaBuffA;
			
			if(line<300)initTable(vgaBuffNext,BUFFER_SIZE,0xFFAA);
			else if(line<600) initTable(vgaBuffNext,BUFFER_SIZE,0xAAFF);
		}
		//line++;
		if(line==625U){
			initTable(vgaBuffNext,BUFFER_SIZE,0xFFAA);
			line=0U;
		}
		//Reset UIF
		TIM2->SR &= ~TIM_SR_UIF;
	}
}

