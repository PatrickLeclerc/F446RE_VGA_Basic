#ifndef MAIN_H
#define MAIN_H

/* Includes */
extern "C"{
#include "drivers.h"
#include "comport.h"
}
#include "vgaObject.h"
#include <math.h>
/* Variables */
volatile uint8_t uartRxFlag=0;
volatile uint8_t vgaFlag=0;
uint8_t vgaScreenBuff[BUFFER_SIZE_Y*BUFFER_SIZE_X] = {};
volatile uint8_t* vgaBuffNext = vgaScreenBuff;
uint8_t vgaScreenBuff2[BUFFER_SIZE_Y*BUFFER_SIZE_X] = {};
uint8_t* vgaCurrentScreenBuff = vgaScreenBuff;
uint8_t* vgaNextScreenBuff = vgaScreenBuff2;

/* Point */
typedef struct{
	float x;
	float y;
} Pt_t;	

/* Functions */
void drawTime(int x, int y);
void drawShape(Pt_t *pts, int size);
Pt_t applyRotation(Pt_t pt, float th);

void drawShape(Pt_t *pts, int size){
	for(int i=0;i<(size-1);i++)
		VGADrawLine(vgaNextScreenBuff,pts[i].x,pts[i].y,pts[i+1].x,pts[i+1].y);
}
Pt_t applyRotation(Pt_t pt, float th){
	/* Apply rotation */
	Pt_t ret;
	ret.x = int(float(pt.x) * cos(th) + float(pt.y) * sin(th));
	ret.y = int(float(pt.y) * cos(th) - float(pt.x) * sin(th));
	return ret;
}

void drawTime(int x, int y){
	char timeChar[6] = {};
	getTime(timeChar);
	int j = 0;
	for(int i=0;i<8;i++){
		if(i==2 || i==5)
			VGAPutChar(vgaNextScreenBuff,x+8*i,y,':');
		else{
			VGAPutChar(vgaNextScreenBuff,x+8*i,y,timeChar[j]+'0');
			j++;
		}
	}
}

#endif
