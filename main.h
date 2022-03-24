#ifndef MAIN_H
#define MAIN_H
#include "drivers.h"
//#include "vga.h"
/*////////////////Defines////////////////*/
#define BUFFER_SIZE (uint32_t) 24U

/*////////////////Variables////////////////*/
extern uint16_t* vgaBuffA;
extern uint16_t* vgaBuffB;
extern uint16_t*  vgaBuffNext;

/*////////////////Functions////////////////*/
/*Declarations*/
void TIM2_IRQHandler(void);
/*Definitions*/
void TIM2_IRQHandler(){
	static uint32_t line = 0U;
	if(TIM2->SR & TIM_SR_UIF){
		/*Handling DMA1_STREAM4_CH0 stuff*/
		DMA1->HIFCR |= DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4;
		DMA1_Stream4->M0AR = (uint32_t)vgaBuffNext;
		DMA1_Stream4->CR |= DMA_SxCR_EN;/*Start*/
		/*Evaluate next line*/
		line++;
		if(line==625U){
			line=0U;
		}
		/*Calculate next buffer once every two lines*/
		if((line&1)==0){
			if(vgaBuffNext==vgaBuffA) vgaBuffNext = vgaBuffB;
			else vgaBuffNext = vgaBuffA;
			updateBuffer(vgaBuffNext, BUFFER_SIZE, line>>1);
		}
		/*Reset UIF*/
		TIM2->SR &= ~TIM_SR_UIF;
	}
}
#endif
