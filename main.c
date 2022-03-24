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

void TIM2_IRQHandler(){
	static uint32_t line = 0U;
	if(TIM2->SR & TIM_SR_UIF){
		/*Handling DMA1_STREAM4_CH0 stuff*/
		DMA1->HIFCR |= DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4;
		DMA1_Stream4->M0AR = (uint32_t)vgaBuffNext;
		DMA1_Stream4->CR |= DMA_SxCR_EN;/*Start*/
		/*For next line*/
		line++;
		if(line==625U){
			line=0U;
		}
		/*Calculate next buffer once every two lines*/
		if((line%2)==0){
			if(vgaBuffNext==vgaBuffA) vgaBuffNext = vgaBuffB;
			else vgaBuffNext = vgaBuffA;
			updateBuffer(vgaBuffNext, BUFFER_SIZE, line>>1);
		}
		/*Reset UIF*/
		TIM2->SR &= ~TIM_SR_UIF;
	}
}

