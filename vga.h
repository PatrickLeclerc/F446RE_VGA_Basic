#ifndef VGA_H
#define VGA_H
#include "stdint.h"
#include "vga_char.h"
/* Functions declaration */
uint8_t* charToVga(char val);
void updateBuffer(uint16_t* table, uint32_t tableSize, uint32_t line);
void initTable(uint16_t* table, uint32_t tableSize, uint16_t value);
#endif
