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

/* VGA Red */
uint8_t vgaScreenBuffR[BUFFER_SIZE_Y*BUFFER_SIZE_X] = {};
uint8_t vgaScreenBuffR2[BUFFER_SIZE_Y*BUFFER_SIZE_X] = {};
volatile uint8_t* vgaBuffNextR = vgaScreenBuffR;
uint8_t* vgaCurrentScreenBuffR = vgaScreenBuffR;
uint8_t* vgaNextScreenBuffR = vgaScreenBuffR2;

/* VGA Green */
uint8_t vgaScreenBuffG[BUFFER_SIZE_Y*BUFFER_SIZE_X] = {};
uint8_t vgaScreenBuffG2[BUFFER_SIZE_Y*BUFFER_SIZE_X] = {};
volatile uint8_t* vgaBuffNextG = vgaScreenBuffG;
uint8_t* vgaCurrentScreenBuffG = vgaScreenBuffG;
uint8_t* vgaNextScreenBuffG = vgaScreenBuffG2;

/* VGA Blue */
uint8_t vgaScreenBuffB[BUFFER_SIZE_Y*BUFFER_SIZE_X] = {};
uint8_t vgaScreenBuffB2[BUFFER_SIZE_Y*BUFFER_SIZE_X] = {};
volatile uint8_t* vgaBuffNextB = vgaScreenBuffB;
uint8_t* vgaCurrentScreenBuffB = vgaScreenBuffB;
uint8_t* vgaNextScreenBuffB = vgaScreenBuffB2;
	
#endif
