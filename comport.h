#ifndef COMPORT_H
#define COMPORT_H
/* Includes */
#include "stm32f446xx.h"

/* Defines */
#define UARTBUFFSIZE (uint32_t)128U
extern uint8_t uartBuff[UARTBUFFSIZE];

/* Print on uart2 test */
void printUart2(char* data);
#endif
