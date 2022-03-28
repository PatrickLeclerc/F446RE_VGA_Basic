#include "vga.h"
void updateBuffer(uint8_t* table, uint32_t tableSize, uint32_t line, char* sentence){
	/*Déphasage pour ne pas écrire sur la bordure*/
	static const unsigned int deph = 2;
	/*Create the back ground*/
	initTable(table,tableSize,0);
	/*We need to prints a character one charLine at a time...*/
	uint32_t charMiniLine = line%8;
	/*Fill the vgaBuffer*/
	unsigned int sentenceLenght = strlen(sentence);
	for(unsigned int i = 0;i<sentenceLenght;i++){
		table[i+deph] |= vgaChar[sentence[i]-32][charMiniLine];
	}
}

uint8_t* charToVga(char val){
	if(val<32) return vgaChar[' '-32];
	else	return vgaChar[val-32];
}

void initTable(uint8_t* table, uint32_t tableSize, uint8_t value){
	uint8_t marge = 1; /*0 ou 1*/
	table[0]=0xA0U*marge;
	for(int i=1;i<(int)tableSize-1;i++){
		table[i]=value;
	}
	table[tableSize-1]=0x05U*marge;
}
