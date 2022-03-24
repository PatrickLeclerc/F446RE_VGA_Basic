#include "vga.h"
void updateBuffer(uint16_t* table, uint32_t tableSize, uint32_t line){
	initTable(table,tableSize,0);
	uint8_t* character = vgaMaj[4];
	if(line<8){
		table[2] |= character[line];
	}
}

void initTable(uint16_t* table, uint32_t tableSize, uint16_t value){
	uint16_t marge = 1; /*0 ou 1*/
	table[0]=0xF5F0U*marge;
	for(int i=1;i<(int)tableSize-1;i++){
		table[i]=value;
	}
	table[tableSize-1]=0x0F5FU*marge;
}

uint8_t* charToVgaMaj(char val){
	return vgaMaj[val-65];
}
