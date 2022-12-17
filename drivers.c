#include "drivers.h"
void startup(uint32_t buffSize){
	/*General Initialisation*/
	initClock(144);
	/*Comport*/
	initUart2(115200);
	/* RTC */
	initRTC();
	/*VGA Initialisation*/
	initSPI1(buffSize);
	initSPI2(buffSize);/*VGAColor*/
	initSPI3(buffSize);
	initVSYNC();/*TIM2*/
	initHSYNC();/*TIM3*/
	/* RTC */
	initRTC();
}

/*144MHz*/
void initClock(uint32_t fPclk){
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
	RCC->PLLCFGR |= (fPclk<<RCC_PLLCFGR_PLLN_Pos);
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

/* VGA */
void initHSYNC(){/*TIM2*/
	/*
		GPIO A0,A1 for ch1,2
		GPIO B10 for ch3
	*/
	
	/* GPIO */
	//RCC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;// | RCC_AHB1ENR_GPIOBEN;
	
	//HSYNC PWM
	GPIOA->MODER |= GPIO_MODER_MODE0_1 | GPIO_MODER_MODE1_1;
	GPIOA->OTYPER |= GPIO_OTYPER_OT0_Msk | GPIO_OTYPER_OT1_Msk;
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL0_0 | GPIO_AFRL_AFRL1_0;
	//GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;//A0 PU
	
	//HSYNC Visible area
	//GPIOB->MODER |= GPIO_MODER_MODE10_1;
	//GPIOB->OTYPER |= GPIO_OTYPER_OT10_Msk;
	//GPIOB->AFR[1] |= GPIO_AFRH_AFRH2_0;	
	
	/*TIM*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CR1 = TIM_CR1_ARPE | TIM_CR1_URS;
	TIM2->CR2 = TIM_CR2_MMS_1;
	TIM2->PSC = 1U;
	TIM2->ARR = 1023U;
	TIM2->CNT = 0U;
	
	/* PWM */
	//* CH2 for positive pulse (NOR) */
	TIM2->CCMR1 |= (7U<<TIM_CCMR1_OC2M_Pos)|TIM_CCMR1_OC2PE;
	TIM2->CCR2 = 800U+24U;
	
	//* CH1 for positive pulse (NOR) */
	TIM2->CCMR1 |= (6U<<TIM_CCMR1_OC1M_Pos)|TIM_CCMR1_OC1PE;
	TIM2->CCR1 = 800U+24U+72U;
	
	//* CH3 visible area : SPI AND CH1 */
	//TIM2->CCMR2 = (6U<<TIM_CCMR2_OC3M_Pos)|TIM_CCMR2_OC3PE;
	//TIM2->CCR3 = 800U+24;
	
	//* Update preloaded registers and enable pwm outputs */
	TIM2->EGR |= TIM_EGR_UG;
	TIM2->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
	//TIM2->CCER |= TIM_CCER_CC3E;
	/* NVIC */
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_SetPriority(TIM2_IRQn,1);
	NVIC_EnableIRQ(TIM2_IRQn);
	
	/* Enable */
	TIM2->CR1 |= TIM_CR1_CEN;
}

void initVSYNC(){/*TIM3*/
	/*
		GPIO A6,A7 for ch1,2
		GPIO B0 for ch3
	*/	
	//RCC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;// | RCC_AHB1ENR_GPIOBEN;
	
	//VSYNC
	GPIOA->MODER |= GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
	GPIOA->OTYPER |= GPIO_OTYPER_OT6_Msk | GPIO_OTYPER_OT7_Msk;
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL6_1 | GPIO_AFRL_AFRL7_1;
	//GPIOA->PUPDR |= GPIO_PUPDR_PUPD6_0;//A6 PU
	
	//VSYNC Visible area
	//GPIOB->MODER |= GPIO_MODER_MODE0_1;
	//GPIOB->OTYPER |= GPIO_OTYPER_OT0_Msk;
	//GPIOB->AFR[0] |= GPIO_AFRL_AFRL0_1;	

	/*TIM*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1 = TIM_CR1_ARPE | TIM_CR1_URS;
	TIM3->SMCR = (1U<<TIM_SMCR_TS_Pos) | (7U<<TIM_SMCR_SMS_Pos);
	TIM3->PSC = 0U;
	TIM3->ARR = 624U;
	TIM3->CNT = 0U;
	
	/* PWM */
	//*CH2 for positive pulse (NOR)*/
	TIM3->CCMR1 |= (7U<<TIM_CCMR1_OC2M_Pos)|TIM_CCMR1_OC2PE;
	TIM3->CCR2 = 600U+1U;
	
	//*CH1 for positive pulse (NOR)*/
	TIM3->CCMR1 |= (6U<<TIM_CCMR1_OC1M_Pos)|TIM_CCMR1_OC1PE;
	TIM3->CCR1 = 600U+1U+2U;
	
	//*CH3 visible area : SPI AND CH*/
	//TIM3->CCMR2 = (6U<<TIM_CCMR2_OC3M_Pos)|TIM_CCMR2_OC3PE;
	//TIM3->CCR3 = 600U+1;
	
	//*Update preloaded registers and enable timer*/
	TIM3->EGR |= TIM_EGR_UG;
	TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
	//TIM3->CCER |= TIM_CCER_CC3E;
	//NVIC
	TIM3->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM3_IRQn);
	/* Enable */
	TIM3->CR1 |= TIM_CR1_CEN;
}

void initSPI1(uint32_t buffSize){/*GPIOB5AF5-DMA2Ch3Stream3ou5*/
	/*RCC*/
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_DMA2EN;
	
	/*GPIO*/
	GPIOB->MODER &= ~GPIO_MODER_MODE5_Msk;
	GPIOB->MODER |= GPIO_MODER_MODE5_1;

	GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5_Msk;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_0;
	//GPIOB->OTYPER |= GPIO_OTYPER_OT5_Msk;
	GPIOB->AFR[0] |= 5U<<GPIO_AFRL_AFSEL5_Pos;
	
	/*SPI2: Internal slave management (master) */
	SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | (1U<<SPI_CR1_BR_Pos);//|SPI_CR1_DFF  //18MHz
	
	/*DMA*/
	DMA2_Stream3->CR = 0U;
	while(DMA2_Stream3->CR != 0U){}
	DMA2_Stream3->PAR = (uint32_t)&(SPI1->DR);
	DMA2_Stream3->NDTR = buffSize;
	DMA2_Stream3->CR |= (3U<<DMA_SxCR_CHSEL_Pos) | DMA_SxCR_PL_Msk | DMA_SxCR_DIR_0 | DMA_SxCR_MINC;//| DMA_SxCR_MSIZE_0 | DMA_SxCR_PSIZE_0
	DMA2_Stream3->FCR = 0U;
	//DMA2_Stream3->CR |= DMA_SxCR_EN;
	/*Enable*/
	SPI1->CR2 = SPI_CR2_TXDMAEN;
	SPI1->CR1 |= SPI_CR1_SPE; 
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
	//GPIOC->OTYPER |= GPIO_OTYPER_OT3_Msk;
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
void initSPI3(uint32_t buffSize){/*GPIOC1AF5-DMA1Ch0Stream7*/
	/*RCC*/
	RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN|RCC_AHB1ENR_DMA1EN;
	
	/*GPIO*/
	GPIOC->MODER &= ~GPIO_MODER_MODE1_Msk;
	GPIOC->MODER |= GPIO_MODER_MODE1_1;
	
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED1_Msk;
	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED1_0;
	//GPIOC->OTYPER |= GPIO_OTYPER_OT1_Msk;
	GPIOC->AFR[0] |= 5U<<GPIO_AFRL_AFSEL1_Pos;
	
	/*SPI2: Internal slave management (master) */
	SPI3->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | (0U<<SPI_CR1_BR_Pos);//|SPI_CR1_DFF  //18MHz
	
	/*DMA*/
	DMA1_Stream7->CR = 0U;
	while(DMA1_Stream7->CR != 0U){}
	DMA1_Stream7->PAR = (uint32_t)&(SPI3->DR);
	DMA1_Stream7->NDTR = buffSize;
	DMA1_Stream7->CR |= (0U<<DMA_SxCR_CHSEL_Pos) | DMA_SxCR_PL_Msk | DMA_SxCR_DIR_0 | DMA_SxCR_MINC;//| DMA_SxCR_MSIZE_0 | DMA_SxCR_PSIZE_0
	DMA1_Stream7->FCR = 0U;
	//DMA1_Stream7->CR |= DMA_SxCR_EN;
	/*Enable*/
	SPI3->CR2 = SPI_CR2_TXDMAEN;
	SPI3->CR1 |= SPI_CR1_SPE; 
}
/* COMPORT */
void initUart2(uint32_t baudrate){
	/* RCC */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
	/* GPIO */
	GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;
	GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);
	GPIOA->OSPEEDR |= (3U<<GPIO_OSPEEDR_OSPEED2_Pos) | (3U<<GPIO_OSPEEDR_OSPEED3_Pos);
	
	/* Usart @ 115200 */
	USART2->CR1 = USART_CR1_TE | USART_CR1_RE;
	USART2->BRR = (SystemCoreClock>>2U) / (baudrate);
	
	/* IRQ */
	USART2->CR1 |= USART_CR1_RXNEIE;
	NVIC_EnableIRQ(USART2_IRQn);
	/* Enable */
	USART2->CR1 |= USART_CR1_UE;
}
/* RTC */
void initRTC(){
	/* PWR */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_DBP;
	while(!(PWR->CR & PWR_CR_DBP)){}
	/* LSI */
	//RCC->CSR |=  RCC_CSR_LSION;
	//while(!(RCC->CSR & RCC_CSR_LSIRDY)){}
	
	///* LSE */
	//RCC->BDCR |=  RCC_BDCR_LSEON;
	//while(!(RCC->BDCR & RCC_BDCR_LSERDY)){}
	
	/* RCC */
	
	RCC->CFGR |= (8 << RCC_CFGR_RTCPRE_Pos);
	RCC->BDCR |= RCC_BDCR_BDRST; //Reset for RCC_BDCR_RTCSEL to be applied
	RCC->BDCR &= ~RCC_BDCR_BDRST;
	RCC->BDCR |= RCC_BDCR_RTCSEL_0 | RCC_BDCR_RTCSEL_1;
	RCC->BDCR |= RCC_BDCR_RTCEN;
	
	/* RTC */
	//Key
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;	
	
	///Init
	RTC->ISR |= RTC_ISR_INIT;
	while(!(RTC->ISR & RTC_ISR_INITF)){}
	
	//Prescaler
	RTC->PRER = (7999<<RTC_PRER_PREDIV_S_Pos);
	RTC->PRER |= (124<<RTC_PRER_PREDIV_A_Pos);
		
	//Adjust time and date
	uint32_t ht = __TIME__[0] - '0';
	uint32_t hu = __TIME__[1] - '0';
	uint32_t mt = __TIME__[3] - '0';
	uint32_t mu = __TIME__[4] - '0';
	uint32_t st = __TIME__[6] - '0';
	uint32_t su = __TIME__[7] - '0';
	RTC->TR = (ht << RTC_TR_HT_Pos) | (hu << RTC_TR_HU_Pos) | (mt << RTC_TR_MNT_Pos) | (mu << RTC_TR_MNU_Pos) | (st << RTC_TR_ST_Pos) | (su << RTC_TR_SU_Pos);
	
	//RTC shadow register
	//RTC->CR |= RTC_CR_BYPSHAD;
	//Start RTC
	RTC->ISR &= ~RTC_ISR_INIT;
	RTC->WPR = 0x00;
}

void getTimeBcd(char* time){
	time[0] = ((RTC->TR & RTC_TR_HT_Msk) >> RTC_TR_HT_Pos);
	time[1] = ((RTC->TR & RTC_TR_HU_Msk) >> RTC_TR_HU_Pos);
	time[2] = ((RTC->TR & RTC_TR_MNT_Msk) >> RTC_TR_MNT_Pos);
	time[3] = ((RTC->TR & RTC_TR_MNU_Msk) >> RTC_TR_MNU_Pos);
	time[4] = ((RTC->TR & RTC_TR_ST_Msk) >> RTC_TR_ST_Pos);
	time[5] = ((RTC->TR & RTC_TR_SU_Msk) >> RTC_TR_SU_Pos);
}

void getTimeNum(int* time){
	time[0] = ((RTC->TR & RTC_TR_HT_Msk) >> RTC_TR_HT_Pos)*10 + ((RTC->TR & RTC_TR_HU_Msk) >> RTC_TR_HU_Pos);
	time[1] = ((RTC->TR & RTC_TR_MNT_Msk) >> RTC_TR_MNT_Pos)*10 + ((RTC->TR & RTC_TR_MNU_Msk) >> RTC_TR_MNU_Pos);
	time[2] = ((RTC->TR & RTC_TR_ST_Msk) >> RTC_TR_ST_Pos)*10 + ((RTC->TR & RTC_TR_SU_Msk) >> RTC_TR_SU_Pos);
}
