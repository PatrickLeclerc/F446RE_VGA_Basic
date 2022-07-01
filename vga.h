#ifndef VGA_H
#define VGA_H
#include "stdint.h"
#include "string.h"

/* Functions declaration */
uint8_t* charToVga(char val);
void VGAcreateFrame(uint8_t* table, uint32_t tableSize, uint32_t line,char* sentence);

#endif
