#include "vga.h"
void updateBuffer(uint16_t* table, uint32_t tableSize, uint32_t line){
	initTable(table,tableSize,0);
	uint8_t* character1 = charToVga('!');
	uint8_t* character2 = charToVga('a');
	uint8_t* character3 = charToVga('A');
	//if(line<8){
		table[2] |= character1[line%8];
		table[3] |= character2[line%8];
		table[4] |= character3[line%8];
	//}
}

uint8_t* charToVga(char val){
	if(val<32) return vgaChar[' '-32];
	else	return vgaChar[val-32];
}

void initTable(uint16_t* table, uint32_t tableSize, uint16_t value){
	uint16_t marge = 0; /*0 ou 1*/
	table[0]=0xF5F0U*marge;
	for(int i=1;i<(int)tableSize-1;i++){
		table[i]=value;
	}
	table[tableSize-1]=0x0F5FU*marge;
}
