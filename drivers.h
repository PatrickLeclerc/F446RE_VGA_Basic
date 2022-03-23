#include "stm32f446xx.h"
#ifndef DRIVERS_H
#define DRIVERS_H
/*Inits ALL*/
void startup(uint16_t* buffer0, uint32_t buffSize);
/*180MHz*/
void initClock(void);
	/*TIM2
			GPIO A0,A1 for ch1,2
			GPIO B10 for ch3
		*/
void initHSYNC(void);
/*TIM3 
		GPIO A6,A7 for ch1,2
		GPIO B0 for ch3
*/
void initVSYNC(void); 
/*18MHz-GPIOC3AF5-DMA1Ch0Stream4*/
void initSPI2(uint16_t* buffer0, uint32_t buffSize);
#endif
