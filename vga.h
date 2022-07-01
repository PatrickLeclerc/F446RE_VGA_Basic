#ifndef VGA_H
#define VGA_H
#include "stdint.h"
#include "string.h"

/* Defines */
#define BUFFER_SIZE_Y  (int)300
#define BUFFER_SIZE_X  (int)48
#define BUFFER_BITSIZE_X  ((int)BUFFER_SIZE_X*8)
	
/* Functions declaration */
uint8_t* charToVga(char val);
void VGACreateFrame(uint8_t table[BUFFER_SIZE_Y][BUFFER_SIZE_X]);
void VGADrawCircle(uint8_t table[BUFFER_SIZE_Y][BUFFER_SIZE_X], int x,int y,int r, int full);
void VGADrawRect(uint8_t table[BUFFER_SIZE_Y][BUFFER_SIZE_X], int x,int y,int r, int full);
#endif
