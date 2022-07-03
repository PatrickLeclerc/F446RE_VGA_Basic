#ifndef MAIN_H
#define MAIN_H
#include "drivers.h"
#include "vga.h"
#include "comport.h"

/*////////////////Variables////////////////*/
volatile uint8_t uartRxFlag=0;
volatile uint8_t vgaFlag=0;
volatile uint8_t vgaScreenBuff[BUFFER_SIZE_Y][BUFFER_SIZE_X] = {};
volatile uint8_t vgaScreenBuff2[BUFFER_SIZE_Y][BUFFER_SIZE_X] = {};
volatile uint8_t* vgaCurrentScreenBuff = vgaScreenBuff;
volatile uint8_t* vgaNextScreenBuff = vgaScreenBuff2;
volatile uint8_t* vgaBuffNext = vgaScreenBuff;

/*////////////////Functions////////////////*/
/*Declarations*/
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void USART2_IRQHandler(void);
	
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
			uint32_t newLine = line >>1;
			if(newLine<BUFFER_SIZE_Y) {
				if(vgaCurrentScreenBuff==vgaScreenBuff) vgaBuffNext = vgaScreenBuff[newLine];
				else vgaBuffNext = vgaScreenBuff2[newLine];
			}
			else vgaBuffNext = vgaCurrentScreenBuff[0];
			
		}

	}
}

void TIM3_IRQHandler(){
	static uint32_t psc = 0U;
	if(TIM3->SR & TIM_SR_UIF){
		/*Reset UIF*/
		TIM3->SR &= ~TIM_SR_UIF;
		psc++;
		/*Evaluate next line*/
		if(psc==1U){
			psc=0U;
			vgaFlag = 1;
			if(vgaCurrentScreenBuff==vgaScreenBuff) {
				vgaCurrentScreenBuff = vgaScreenBuff2;
				vgaNextScreenBuff = vgaScreenBuff;
			}
			else {
				vgaCurrentScreenBuff = vgaScreenBuff;
				vgaNextScreenBuff = vgaScreenBuff2;
			}
		}
		
	}
}

void USART2_IRQHandler(){
	static uint32_t len = 0;
	/* RX */
	if(USART2->SR & USART_SR_RXNE){
		USART2->SR &= ~USART_SR_RXNE;
		if(len < UARTBUFFSIZE){
			uartBuff[len] = (uint8_t)USART2->DR;
			if(uartBuff[len] == '\n'){
				uartRxFlag = 1;
				len = 0;
			}
			else{
				len++;
			}
		}
		else{
			uartBuff[UARTBUFFSIZE-1] = (uint8_t)'\n';
			uartRxFlag = 1;
			len = 0;
		}
	}
}
#endif
