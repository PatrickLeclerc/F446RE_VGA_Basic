#include "vga.h"
#include "vga_char.h"

void updateBuffer(uint8_t* table, uint32_t tableSize, uint32_t line, char* sentence){//46x36
	
	/*Create the static back ground*/
	table[0]=0xF0U;
	for(int i=1;i<(int)tableSize-1;i++){
		table[i]=0;
	}
	table[tableSize-1]=0x0FU;
}

uint8_t* charToVga(char val){
	if(val<32) 	return vgaChar[' '-32];
	else				return vgaChar[val-32];
}

