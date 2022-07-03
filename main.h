#ifndef MAIN_H
#define MAIN_H

#include "drivers.h"
#include "vga.h"
#include "comport.h"

/* Variables */
volatile uint8_t uartRxFlag=0;
volatile uint8_t vgaFlag=0;
volatile uint8_t vgaScreenBuff[BUFFER_SIZE_Y][BUFFER_SIZE_X] = {};
volatile uint8_t vgaScreenBuff2[BUFFER_SIZE_Y][BUFFER_SIZE_X] = {};
volatile uint8_t* vgaCurrentScreenBuff = vgaScreenBuff;
volatile uint8_t* vgaNextScreenBuff = vgaScreenBuff2;
volatile uint8_t* vgaBuffNext = vgaScreenBuff;
#endif
