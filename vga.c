#include "vga.h"
#include "vga_char.h"

uint8_t* charToVga(char val){
	if(val<32) 	return vgaChar[' '-32];
	else				return vgaChar[val-32];
}

void VGAcreateFrame(uint8_t* table, uint32_t tableSize, uint32_t line, char* sentence){//46x36
	
	/*Create the static back ground*/
	if(line<2 || line>297){
		for(int i=0;i<(int)tableSize;i++){
			table[i]=0xFF;
		}
	}
	else{
		table[0]=0x80;
		for(int i=1;i<(int)tableSize-1;i++){
			table[i]=0;
		}
		table[tableSize-1]=0x01U;
	}
}

