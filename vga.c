#include "vga.h"

//void updateBuffer(uint8_t* table, uint32_t tableSize, uint32_t line, char* sentence){//46x36
//	/*Déphasage pour ne pas écrire sur la bordure*/
//	static const unsigned int deph = 1;
//	/*Create the back ground*/
//	initTable(table,tableSize,line);
//	/*We need to prints a character one charLine at a time...*/
//	uint32_t charMiniLine = line%8;
//	/*Fill the vgaBuffer*/
//	unsigned int sentenceLenght = strlen(sentence);
//	for(unsigned int i = 0;i<sentenceLenght;i++){
//		table[i+deph] |= vgaChar[sentence[i]-32][charMiniLine];
//	}
//}

void updateBuffer(uint8_t* table, uint32_t tableSize, uint32_t line, char* sentence){//46x36
	/*Create the static back ground*/
	initTable(table,tableSize,line);
	
}

uint8_t* charToVga(char val){
	if(val<32) return vgaChar[' '-32];
	else	return vgaChar[val-32];
}

void initTable(uint8_t* table, uint32_t tableSize, uint32_t line){/*Generates frames*/
	/*Cadre*/
	table[0]=0xF0U;
	for(int i=1;i<(int)tableSize-1;i++){
		table[i]=0;
	}
	table[tableSize-1]=0x0FU;
	/*Cadre de tetris, Bottom left corner*/
	static int posX = TETRIS_POS_X;
	static int posY = TETRIS_POS_Y<<3;
	if(((line==posY-1) || (line==(posY+(22*8)+1)))){
		for(int i=posX;i<(int)(posX+10);i++){
			table[i]|=0xAA;
		}
	}
	else if(((line>posY-1) && (line<posY+(22*8)+1))){
		if((line+posX)&1){
			table[posX-1]|=0x1;
			table[posX+10]|=0x80;
		}
	}
}

