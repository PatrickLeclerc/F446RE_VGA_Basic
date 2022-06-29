#include "drivers.h"
void startup(uint32_t buffSize){
	/*General Initialisation*/
	initClock();
	/*VGA Initialisation*/
	initSPI2(buffSize);/*VGAColor1*/
	initVSYNC();/*TIM2*/
	initHSYNC();/*TIM3*/
}
/*144MHz*/
void initClock(){
	/*APB1 45MHz Max*/
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
	/*APB2 90MHz Max*/
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
	/*HSE*/
	RCC->CR = (RCC->CR & ~RCC_CR_HSEBYP) | RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY)){}
	/*PLLSRC*/
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	/*PLLM*/
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM_Msk;
	RCC->PLLCFGR |= (4U<<RCC_PLLCFGR_PLLM_Pos);
	/*PLLN*/
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_Msk;
	RCC->PLLCFGR |= (144U<<RCC_PLLCFGR_PLLN_Pos);
	/*PLLP*/
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP_Msk;
	/*FLASHMEM*/
	FLASH->ACR |= FLASH_ACR_PRFTEN|FLASH_ACR_LATENCY_5WS;
	/*Activate PLL*/
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY)){}
	/*Switch clock source to PLL*/
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	SystemCoreClockUpdate();
}

void initHSYNC(){/*TIM2*/ // GPIOA0 is pulled up
	/*
		GPIO A0,A1 for ch1,2
		GPIO B10 for ch3
	*/
	/* GPIO */
	//RCC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
	
	//HSYNC PWM
	GPIOA->MODER |= GPIO_MODER_MODE0_1 | GPIO_MODER_MODE1_1;
	GPIOA->OTYPER |= GPIO_OTYPER_OT0_Msk | GPIO_OTYPER_OT1_Msk;
#warning should the pin be pulled-up exernaly?
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0; 
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL0_0 | GPIO_AFRL_AFRL1_0;
	
	//HSYNC Visible area
	GPIOB->MODER |= GPIO_MODER_MODE10_1;
	GPIOB->OTYPER |= GPIO_OTYPER_OT10_Msk;
	GPIOB->AFR[1] |= GPIO_AFRH_AFRH2_0;	
	
	/*TIM*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CR1 = TIM_CR1_ARPE | TIM_CR1_URS;
	TIM2->CR2 = TIM_CR2_MMS_1;
	TIM2->PSC = 1U;
	TIM2->ARR = 1023U;
	TIM2->CNT = 0U;
	
	/* PWM */
	//* CH1 for positive pulse (NOR) */
	TIM2->CCMR1 = (7U<<TIM_CCMR1_OC1M_Pos)|TIM_CCMR1_OC1PE;
	TIM2->CCR1 = 800U+24U+72U;
	
	//* CH2 for positive pulse (NOR) */
	TIM2->CCMR1 |= (6U<<TIM_CCMR1_OC2M_Pos)|TIM_CCMR1_OC2PE;
	TIM2->CCR2 = 800U+24U;
	
	//* CH3 visible area : SPI AND CH1 */
	TIM2->CCMR2 = (7U<<TIM_CCMR2_OC3M_Pos)|TIM_CCMR2_OC3PE;
	TIM2->CCR3 = 800U;
	
	//* Update preloaded registers and enable pwm outputs */
	TIM2->EGR |= TIM_EGR_UG;
	TIM2->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E;
	
	/* NVIC */
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);
	
	/* Enable */
	TIM2->CR1 |= TIM_CR1_CEN;
}

void initVSYNC(){/*TIM3*/ // GPIOA6 is pulled up
	/*
		GPIO A6,A7 for ch1,2
		GPIO B0 for ch3
	*/	
	//RCC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
	
	//VSYNC
	GPIOA->MODER |= GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
	GPIOA->OTYPER |= GPIO_OTYPER_OT6_Msk | GPIO_OTYPER_OT7_Msk;
#warning should the pin be pulled-up exernaly?
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD6_0; 
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL6_1 | GPIO_AFRL_AFRL7_1;
	
	//VSYNC Visible area
	GPIOB->MODER |= GPIO_MODER_MODE0_1;
	GPIOB->OTYPER |= GPIO_OTYPER_OT0_Msk;
	GPIOB->AFR[0] |= GPIO_AFRL_AFRL0_1;	

	/*TIM*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1 = TIM_CR1_ARPE | TIM_CR1_URS;
	TIM3->SMCR = (1U<<TIM_SMCR_TS_Pos) | (7U<<TIM_SMCR_SMS_Pos);
	TIM3->PSC = 0U;
	TIM3->ARR = 624U;
	TIM3->CNT = 0U;
	
	/* PWM */
	//*CH1 for positive pulse (NOR)*/
	TIM3->CCMR1 = (7U<<TIM_CCMR1_OC1M_Pos)|TIM_CCMR1_OC1PE;
	TIM3->CCR1 = 600U+1U+2U;
	
	//*CH2 for positive pulse (NOR)*/
	TIM3->CCMR1 |= (6U<<TIM_CCMR1_OC2M_Pos)|TIM_CCMR1_OC2PE;
	TIM3->CCR2 = 600U+1U;
	
	//*CH3 visible area : SPI AND CH1*/
	TIM3->CCMR2 = (7U<<TIM_CCMR2_OC3M_Pos)|TIM_CCMR2_OC3PE;
	TIM3->CCR3 = 600U;
	
	//*Update preloaded registers and enable timer*/
	TIM3->EGR |= TIM_EGR_UG;
	TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E;
	
	/* Enable */
	TIM3->CR1 |= TIM_CR1_CEN;
}

void initSPI2(uint32_t buffSize){/*GPIOC3AF5-DMA1Ch0Stream4*/
	/*RCC*/
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN|RCC_AHB1ENR_DMA1EN;
	
	/*GPIO*/
	GPIOC->MODER &= ~GPIO_MODER_MODE3_Msk;
	GPIOC->MODER |= GPIO_MODER_MODE3_1;
	
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED3_Msk;
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED3_0;
	GPIOC->OTYPER |= GPIO_OTYPER_OT3_Msk;
#warning should the pin be pulled-up exernaly?
	GPIOC->PUPDR 	|= GPIO_PUPDR_PUPD3_0;
	GPIOC->AFR[0] |= 5U<<GPIO_AFRL_AFSEL3_Pos;
	
	/*SPI2: Internal slave management (master) */
	SPI2->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | (0U<<SPI_CR1_BR_Pos);//|SPI_CR1_DFF  //18MHz
	
	/*DMA*/
	DMA1_Stream4->CR = 0U;
	while(DMA1_Stream4->CR != 0U){}
	DMA1_Stream4->PAR = (uint32_t)&(SPI2->DR);
	DMA1_Stream4->NDTR = buffSize;
	DMA1_Stream4->CR |= (0U<<DMA_SxCR_CHSEL_Pos) | DMA_SxCR_PL_Msk | DMA_SxCR_DIR_0 | DMA_SxCR_MINC;//| DMA_SxCR_MSIZE_0 | DMA_SxCR_PSIZE_0
	DMA1_Stream4->FCR = 0U;
	//DMA1_Stream4->CR |= DMA_SxCR_EN;
	/*Enable*/
	SPI2->CR2 = SPI_CR2_TXDMAEN;
	SPI2->CR1 |= SPI_CR1_SPE; 
}
