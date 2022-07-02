#include "comport.h"
/* Variables */
uint8_t uartBuff[UARTBUFFSIZE] = {0};

/* Functions */
void printUart2(char* data){
	int size = (int) strlen(data);
	for(int i=0;i<size;i++){
		while(!(USART2->SR & USART_SR_TXE)){}
		USART2->DR = data[i];
	}
}
