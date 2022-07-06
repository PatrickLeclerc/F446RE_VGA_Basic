#ifndef VGA_H
#define VGA_H
#include "stdint.h"
#include "string.h"

/* Defines */
#define BUFFER_SIZE_Y  (int)300
#define BUFFER_SIZE_X  (int)50
#define BUFFER_BITSIZE_X  ((int)BUFFER_SIZE_X*8)

/* Functions declaration */
uint8_t* charToVga(char val);
void VGACreateFrame(uint8_t* table);
void VGADrawCircle(uint8_t* table, int x,int y,int r, int full);
void VGADrawRect(uint8_t* table, int x,int y,int RX, int RY, int full);
void VGADrawLine(uint8_t* table, int x1,int y1,int x2,int y2);
void VGAPutChar(uint8_t* table, int X,int Y,char alpha);
#endif
