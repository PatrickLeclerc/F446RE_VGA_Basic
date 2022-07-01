#include "vga.h"
#include "vga_char.h"

void updateBuffer(uint8_t* table, uint32_t tableSize, uint32_t line, char* sentence){//46x36
	
	/*Create the static back ground*/
	table[0]=0xA5;
	for(int i=1;i<(int)tableSize-1;i++){
		table[i]=0;
	}
	table[tableSize-1]=0x0A5U;
	
	/*Vga stuff*/
	uint8_t* character = charToVga('D');
	if(line<8){
		table[2] |= character[line];
	}
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
	if(val<32) 	return vgaChar[' '-32];
	else				return vgaChar[val-32];
}

