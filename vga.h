#ifndef VGA_H
#define VGA_H
#include "stdint.h"
#include "string.h"
#include "vga_char.h"
/* Defines */
#define TETRIS_POS_X 4
#define TETRIS_POS_Y 15
/* Functions declaration */
uint8_t* charToVga(char val);
void updateBuffer(uint8_t* table, uint32_t tableSize, uint32_t line,char* sentence);
void initTable(uint8_t* table, uint32_t tableSize, uint32_t line);
#endif
