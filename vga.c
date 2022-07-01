#include "vga.h"
#include "vga_char.h"

uint8_t* charToVga(char val){
	if(val<32) 	return vgaChar[' '-32];
	else				return vgaChar[val-32];
}

void VGACreateFrame(uint8_t table[BUFFER_SIZE_Y][BUFFER_SIZE_X]){//48*300
	for(int Y = 0; Y < BUFFER_SIZE_Y; Y++){
		/*Create the static back ground*/
		if(Y<1 || Y>=(BUFFER_SIZE_Y-1)){
			for(int i=0;i<(int)BUFFER_SIZE_X;i++){
				table[Y][i]=0xFF;
			}
		}
		else{
			table[Y][0]=0x80;
			for(int i=1;i<(int)BUFFER_SIZE_X-1;i++){
				table[Y][i]=0;
			}
			table[Y][BUFFER_SIZE_X-1]=0x01U;
		}
	}
}

void VGADrawCircle(uint8_t table[BUFFER_SIZE_Y][BUFFER_SIZE_X], int X,int Y,int R, int full){
	int R2 = R-1;
	for(int y = 0; y < BUFFER_SIZE_Y; y++){
		for(int x = 0; x < BUFFER_SIZE_X; x++){
			
			/* Draw circle */
			for(int modX = 0; modX < 8; modX++){
				int realX = x*8+modX;
				int condLeft = (X-realX)*(X-realX) + (Y-y)*(Y-y);
				int condR = condLeft < R*R;
				if(!full){
					condR = condR && (condLeft > R2*R2);
				}
				if(condR){
					table[y][x] |= (1 << (7-modX));
				}
			}
		}
	}
}

void VGADrawRect(uint8_t table[BUFFER_SIZE_Y][BUFFER_SIZE_X], int X,int Y,int R, int full){
	if(!full){
		/* Horizontal */
		for(int x = 0; x < BUFFER_SIZE_X; x++){
			for(int modX = 0; modX < 8; modX++){
				int realX = x*8+modX;
				int condX = (realX>X) && (realX<=(X+R));
				if(condX){
					table[Y][x] |= (1 << (7-modX));
					table[Y+R][x] |= (1 << (7-modX));
				}
			}
		}
		/* Vertical */
		int X1 = X/8;
		int modX1 = X%8;
		int X2 = (X+R)/8;
		int modX2 = (X+R)%8;
		for(int y = 0; y < BUFFER_SIZE_Y; y++){
				int condY = (y>Y) && (y<=(Y+R));
				if(condY){
					table[y][X1] |= (1 << (7-modX1));
					table[y][X2] |= (1 << (7-modX2));
				}
		}
	}
	else{
		for(int y = 0; y < BUFFER_SIZE_Y; y++){
			for(int x = 0; x < BUFFER_SIZE_X; x++){
				for(int modX = 0; modX < 8; modX++){
					int condY = (y>Y) && (y<=(Y+R));
					int realX = x*8+modX;
					int condX = (realX>X) && (realX<=(X+R));
					if(condX && condY){
						table[y][x] |= (1 << (7-modX));
					}
				}
			}
		}
	}
}

void VGAPutChar(uint8_t table[BUFFER_SIZE_Y][BUFFER_SIZE_X], int X,int Y,char alpha){
	uint8_t* character = charToVga(alpha);
	for(int y = 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			/* Pixel value */
			int px = (character[y] >> x)&1;
			
			/* Pixel position */
			int xPx = (X+x)/8;
			//int smallPxPos = x;
			table[Y+y][xPx] |= (px<<x);
		}
	}
}
