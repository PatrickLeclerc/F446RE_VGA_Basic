#include "vga.h"
#include "vga_char.h"

uint8_t* charToVga(char val){
	if(val<32) 	return vgaChar[' '-32];
	else				return vgaChar[val-32];
}

void VGABlankScreen(uint8_t* table){
	memset(table,0,BUFFER_SIZE_X*BUFFER_SIZE_Y);
}


void VGApx(uint8_t* table, int x,int y,VGApxOp_t op){
	if((x>=0) && (x<BUFFER_BITSIZE_X) && (y>=0) && (y<BUFFER_SIZE_Y))
		switch(op){
			case RESET :{
				table[y*BUFFER_SIZE_X + (x/8)] &= ~(1 << (7-(x%8)));
				break;
			}
			case SET :{
				table[y*BUFFER_SIZE_X + (x/8)] |= (1 << (7-(x%8)));
				break;
			}
			case INV :{
				table[y*BUFFER_SIZE_X + (x/8)] ^= (1 << (7-(x%8)));
				break;
			}
		}
		
}

void VGACreateFrame(uint8_t* table){
	VGADrawRect(table,0,0,BUFFER_SIZE_X*8-2,BUFFER_SIZE_Y-2,0);
}

void VGADrawCircle(uint8_t* table, int X,int Y,int R, int full){
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
					table[y*BUFFER_SIZE_X+x] ^= (1 << (7-modX));
				}
			}
		}
	}
}

void VGADrawRect(uint8_t* table, int X,int Y,int RX, int RY, int full){
	if(!full){
		/* Horizontal */
		for(int x = 0; x < BUFFER_SIZE_X; x++){
			for(int modX = 0; modX < 8; modX++){
				int realX = x*8+modX;
				int condX = (realX>X) && (realX<=(X+RX));
				if(condX){
					table[Y*BUFFER_SIZE_X+x] ^= (1 << (7-modX));
					table[(Y+RY)*BUFFER_SIZE_X+x] ^= (1 << (7-modX));
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
					table[y*BUFFER_SIZE_X+X1] ^= (1 << (7-modX1));
					table[y*BUFFER_SIZE_X+X2] ^= (1 << (7-modX2));
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
						table[y*BUFFER_SIZE_X+x] ^= (1 << (7-modX));
					}
				}
			}
		}
	}
}

void VGADrawLine(uint8_t* table, int x1,int y1,int x2,int y2){
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
		if(y2<y1){
			ya = y2;
			yb = y1;
		}

		for(int y = ya; y <= yb; y++){
		if(x<BUFFER_SIZE_X && y<BUFFER_SIZE_Y)
			table[y*BUFFER_SIZE_X+x] ^= (1 << (7-modX));
		}
	}
	else{
		for(int x = 0; x <= xb-xa ; x++){
			int realX = xa + x;

			int y = (yb-ya)*x;
			y /= xb-xa;
			y+= ya;	
			if(realX<BUFFER_BITSIZE_X && y<BUFFER_SIZE_Y)
				table[y*BUFFER_SIZE_X+realX/8] ^= (1 << (7-(realX%8)));
		}
	}
}

void VGAPutChar(uint8_t* table, int X,int Y,char alpha){
	uint8_t* character = charToVga(alpha);
	for(int y = 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			/* Pixel value */
			int px = (character[y] >>(7-x))&1;
			
			/* Pixel position */
			if(px)
				VGApx(table,X+x,y+Y,INV);
		}
	}
}
