#include "isr.h"
#include "vga.h"
#include "comport.h"

/* Extern */
extern volatile uint8_t uartRxFlag;
extern volatile uint8_t vgaFlag;
extern uint8_t vgaScreenBuff[BUFFER_SIZE_Y*BUFFER_SIZE_X];
extern uint8_t vgaScreenBuff2[BUFFER_SIZE_Y*BUFFER_SIZE_X];
extern uint8_t* vgaCurrentScreenBuff;
extern uint8_t* vgaNextScreenBuff;
extern volatile uint8_t* vgaBuffNext;
	
/*Definitions*/
void TIM2_IRQHandler(){
	static uint32_t line = 0U;
	if(TIM2->SR & TIM_SR_UIF){
		/*Handling DMA1_STREAM4_CH0 stuff*/	//SPI2
		DMA1->HIFCR |= DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4;
		DMA1_Stream4->M0AR = (uint32_t)vgaBuffNext;
		/*Handling DMA1_STREAM7_CH0 stuff*/ //SPI3
		DMA1->HIFCR |= DMA_HIFCR_CTCIF7 | DMA_HIFCR_CHTIF7;
		DMA1_Stream7->M0AR = (uint32_t)vgaBuffNext;
		
		/* Start transfers */
		RCC->AHB1ENR &= ~RCC_AHB1ENR_DMA1EN;
		DMA1_Stream4->CR |= DMA_SxCR_EN;/*Start*/
		DMA1_Stream7->CR |= DMA_SxCR_EN;/*Start*/
		RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
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
				if(vgaCurrentScreenBuff==vgaScreenBuff) vgaBuffNext = &vgaScreenBuff[newLine*BUFFER_SIZE_X];
				else vgaBuffNext = &vgaScreenBuff2[newLine*BUFFER_SIZE_X];
			}
			else vgaBuffNext = &vgaCurrentScreenBuff[0];
			
		}

	}
}

void TIM3_IRQHandler(){
	static uint32_t psc = 0U;
	if(TIM3->SR & TIM_SR_UIF){
		/*Reset UIF*/
		TIM3->SR &= ~TIM_SR_UIF;
		
		/*Evaluate next line*/
		if(psc>=1U){
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
		psc++;
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