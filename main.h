#ifndef MAIN_H
#define MAIN_H

/* Includes */
extern "C"{
#include "drivers.h"
#include "comport.h"
#include "vga.h"
}
#include "draw.h"

/* Variables */
volatile uint8_t uartRxFlag=0;
volatile uint8_t vgaFlag=0;
uint8_t vgaScreenBuff[BUFFER_SIZE_Y*BUFFER_SIZE_X] = {};
volatile uint8_t* vgaBuffNext = vgaScreenBuff;
uint8_t vgaScreenBuff2[BUFFER_SIZE_Y*BUFFER_SIZE_X] = {};
uint8_t* vgaCurrentScreenBuff = vgaScreenBuff;
uint8_t* vgaNextScreenBuff = vgaScreenBuff2;

#endif
