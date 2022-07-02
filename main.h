#ifndef MAIN_H
#define MAIN_H
#include "drivers.h"
#include "vga.h"
#include "comport.h"

/*////////////////Variables////////////////*/
volatile uint8_t uartRxFlag=0;
uint8_t vgaScreenBuff[300][48] = {};
uint8_t vgaScreenBuff2[300][48] = {};
uint8_t* vgaBuffA;
uint8_t* vgaBuffB;
extern uint8_t*  vgaBuffNext;

/*////////////////Functions////////////////*/
/*Declarations*/
void TIM2_IRQHandler(void);
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
			vgaBuffNext = vgaScreenBuff[newLine];
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
