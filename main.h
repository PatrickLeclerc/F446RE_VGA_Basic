#ifndef MAIN_H
#define MAIN_H
#include "drivers.h"
#include "vga.h"

/*////////////////Defines////////////////*/
#define BUFFER_SIZE (uint32_t) 48U

/*////////////////Variables////////////////*/
uint8_t vgaScreenBuff[300][48] = {};
extern uint8_t* vgaBuffA;
extern uint8_t* vgaBuffB;
extern uint8_t*  vgaBuffNext;

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
		/*Reset UIF*/
		TIM2->SR &= ~TIM_SR_UIF;
		/*Evaluate next line*/
		line++;
		if(line==625U){
			line=0U;
		}
		/*Calculate next buffer once every two lines*/
		if((line&1)==0){
			if(vgaBuffNext==vgaBuffA) vgaBuffNext = vgaBuffB;
			else vgaBuffNext = vgaBuffA;
			uint32_t newLine = line >>1;
			VGAcreateFrame(vgaBuffNext, BUFFER_SIZE, newLine,	" Study hard what interests you the most in the ");
			//switch(line>>4){
			//	case 7:{updateBuffer(vgaBuffNext, BUFFER_SIZE, newLine,	" Study hard what interests you the most in the ");break;}
			//	case 8:{updateBuffer(vgaBuffNext, BUFFER_SIZE,newLine,	"  most undisciplined, irreverent and original ");break;}
			//	case 9:{updateBuffer(vgaBuffNext, BUFFER_SIZE,newLine,	"  manner possible.          -Richard Feynmann");break;}
			//	default: {updateBuffer(vgaBuffNext, BUFFER_SIZE, newLine, "");break;}
			//}
		}

	}
}
#endif
