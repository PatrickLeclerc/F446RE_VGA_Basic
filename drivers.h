#ifndef DRIVERS_H
#define DRIVERS_H
#include "stm32f446xx.h"
/*Inits ALL*/
void startup(uint32_t buffSize);
/*180MHz*/
void initClock(uint32_t fPclk);
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
/*Color*/
void initSPI2(uint32_t buffSize);
void initSPI3(uint32_t buffSize);
/*USART2*/
void initUart2(uint32_t baudrate);
/*RTC*/

void initRTC(void);
void getTimeBcd(char* time); // HH.MM.SS
void getTimeNum(int* time); // H.M.S
#endif
