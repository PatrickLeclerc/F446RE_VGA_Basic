#include "vga.h"
#include "vga_char.h"

uint8_t* charToVga(char val){
	if(val<32) 	return vgaChar[' '-32];
	else				return vgaChar[val-32];
}

void VGACreateFrame(uint8_t table[BUFFER_SIZE_Y][BUFFER_SIZE_X]){
	for(int y = 0; y < BUFFER_SIZE_Y; y++)
		for(int x = 0; x < BUFFER_SIZE_X; x++)
			table[y][x] = 0;
	VGADrawRect(table,0,0,BUFFER_SIZE_X*8-2,BUFFER_SIZE_Y-2,0);
}

void VGADrawCircle(uint8_t table[BUFFER_SIZE_Y][BUFFER_SIZE_X], int X,int Y,int R, int full){
	int R2 = R-1;
	for(int y = Y-R; y < Y+R; y++){
		for(int x = ((X-R)/8); x <= ((X+R)/8); x++){
			
			/* Draw circle */
			for(int modX = 0; modX < 8; modX++){
				int realX = x*8+modX;
				int condLeft = (X-realX)*(X-realX) + (Y-y)*(Y-y);
				int condR = condLeft < R*R;
				if(!full){
					condR = condR && (condLeft > R2*R2);
				}
				if(condR){
					table[y][x] ^= (1 << (7-modX));
				}
			}
		}
	}
}

void VGADrawRect(uint8_t table[BUFFER_SIZE_Y][BUFFER_SIZE_X], int X,int Y,int RX, int RY, int full){
	if(!full){
		/* Horizontal */
		for(int x = 0; x < BUFFER_SIZE_X; x++){
			for(int modX = 0; modX < 8; modX++){
				int realX = x*8+modX;
				int condX = (realX>X) && (realX<=(X+RX));
				if(condX){
					table[Y][x] |= (1 << (7-modX));
					table[Y+RY][x] |= (1 << (7-modX));
				}
			}
		}
		/* Vertical */
		int X1 = X/8;
		int modX1 = X%8;
		int X2 = (X+RX)/8;
		int modX2 = (X+RX)%8;
		for(int y = 0; y < BUFFER_SIZE_Y; y++){
				int condY = (y>Y) && (y<=(Y+RY));
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
					int condY = (y>Y) && (y<=(Y+RY));
					int realX = x*8+modX;
					int condX = (realX>X) && (realX<=(X+RX));
					if(condX && condY){
						table[y][x] |= (1 << (7-modX));
					}
				}
			}
		}
	}
}

void VGADrawLine(uint8_t table[BUFFER_SIZE_Y][BUFFER_SIZE_X], int x1,int y1,int x2,int y2){
	int xa = x1;
	int xb = x2;
	int ya = y1;
	int yb = y2;
	
	if(x1>x2){
		xa = x2;
		xb = x1;
		ya = y2;
		yb = y1;
	}
	if(x1==x2){
		int x = x1/8; 
		int modX = x1%8;
		for(int y = y1; y <= y2; y++){
		if(x<BUFFER_SIZE_X && y<BUFFER_SIZE_Y)
			table[y][x] ^= (1 << (7-modX));
		}
	}
	else{
		for(int x = xa; x < xb/8; x++){
			for(int modX = 0; modX < 8; modX++){
				int realX = x*8+modX;
				int y = (yb-ya)*realX;
				y /= xb-xa;
				y += ya;
				if(x<BUFFER_SIZE_X && y<BUFFER_SIZE_Y)
					table[y][x] ^= (1 << (7-modX));
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
