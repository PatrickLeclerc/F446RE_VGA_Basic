#include "isr.h"
#include "vga.h"
#include "comport.h"

/* Extern */
extern volatile uint8_t uartRxFlag;
extern volatile uint8_t vgaFlag;

/* VGA Red */
extern uint8_t vgaScreenBuffR[BUFFER_SIZE_Y*BUFFER_SIZE_X];
extern uint8_t vgaScreenBuffR2[BUFFER_SIZE_Y*BUFFER_SIZE_X];
extern volatile uint8_t* vgaBuffNextR;
extern uint8_t* vgaCurrentScreenBuffR;
extern uint8_t* vgaNextScreenBuffR;

/* VGA Green */
extern uint8_t vgaScreenBuffG[BUFFER_SIZE_Y*BUFFER_SIZE_X];
extern uint8_t vgaScreenBuffG2[BUFFER_SIZE_Y*BUFFER_SIZE_X];
extern volatile uint8_t* vgaBuffNextG;
extern uint8_t* vgaCurrentScreenBuffG;
extern uint8_t* vgaNextScreenBuffG;

/* VGA Blue */
extern uint8_t vgaScreenBuffB[BUFFER_SIZE_Y*BUFFER_SIZE_X];
extern uint8_t vgaScreenBuffB2[BUFFER_SIZE_Y*BUFFER_SIZE_X];
extern volatile uint8_t* vgaBuffNextB;
extern uint8_t* vgaCurrentScreenBuffB;
extern uint8_t* vgaNextScreenBuffB;
	
/*Definitions*/
void TIM2_IRQHandler(){
	static uint32_t line = 0U;
	if(TIM2->SR & TIM_SR_UIF){
		/*Handling DMA1_STREAM4_CH0 stuff*/	//SPI2
		DMA1->HIFCR |= DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4;
		DMA1_Stream4->M0AR = (uint32_t)vgaBuffNextR;
		/*Handling DMA1_STREAM7_CH0 stuff*/ //SPI3
		DMA1->HIFCR |= DMA_HIFCR_CTCIF7 | DMA_HIFCR_CHTIF7;
		DMA1_Stream7->M0AR = (uint32_t)vgaBuffNextG;
		/*Handling DMA2_STREAM3_CH3 stuff*/ //SPI1
		DMA2->LIFCR |= DMA_LIFCR_CTCIF3 | DMA_LIFCR_CHTIF3;
		DMA2_Stream3->M0AR = (uint32_t)vgaBuffNextB;
		/* Start transfers */
		DMA1_Stream7->CR |= DMA_SxCR_EN;/*Start*/
		DMA1_Stream4->CR |= DMA_SxCR_EN;/*Start*/
		DMA2_Stream3->CR |= DMA_SxCR_EN;/*Start*/
		
		/* Retart DMAs at the same time for better synch */
		RCC->AHB1ENR &= ~(RCC_AHB1ENR_DMA1EN|RCC_AHB1ENR_DMA2EN);
		RCC->AHB1ENR |= (RCC_AHB1ENR_DMA1EN|RCC_AHB1ENR_DMA2EN);
		
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
				if(vgaCurrentScreenBuffR==vgaScreenBuffR){
					vgaBuffNextR = &vgaScreenBuffR[newLine*BUFFER_SIZE_X];
					vgaBuffNextG = &vgaScreenBuffG[newLine*BUFFER_SIZE_X];
					vgaBuffNextB = &vgaScreenBuffB[newLine*BUFFER_SIZE_X];
				}
				else{
					vgaBuffNextR = &vgaScreenBuffR2[newLine*BUFFER_SIZE_X];
					vgaBuffNextG = &vgaScreenBuffG2[newLine*BUFFER_SIZE_X];
					vgaBuffNextB = &vgaScreenBuffB2[newLine*BUFFER_SIZE_X];
				}
			}
			else{
				vgaBuffNextR = &vgaCurrentScreenBuffR[0];
				vgaBuffNextG = &vgaCurrentScreenBuffG[0];
				vgaBuffNextB = &vgaCurrentScreenBuffB[0];
			}
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
			if(vgaCurrentScreenBuffR==vgaScreenBuffR) {
				vgaCurrentScreenBuffR = vgaScreenBuffR2;
				vgaNextScreenBuffR = vgaScreenBuffR;
				vgaCurrentScreenBuffG = vgaScreenBuffG2;
				vgaNextScreenBuffG = vgaScreenBuffG;
				vgaCurrentScreenBuffB = vgaScreenBuffB2;
				vgaNextScreenBuffB = vgaScreenBuffB;
			}
			else {
				vgaCurrentScreenBuffR = vgaScreenBuffR;
				vgaNextScreenBuffR = vgaScreenBuffR2;
				vgaCurrentScreenBuffG = vgaScreenBuffG;
				vgaNextScreenBuffG = vgaScreenBuffG2;
				vgaCurrentScreenBuffB = vgaScreenBuffB;
				vgaNextScreenBuffB = vgaScreenBuffB2;
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