#ifndef MAIN_H
#define MAIN_H

/* Includes */
extern "C"{
#include "drivers.h"
#include "comport.h"
}
#include "vgaObject.h"

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
void drawShape(Pt_t *pts, int size);

void drawShape(Pt_t *pts, int size){
	for(int i=0;i<(size-1);i++)
		VGADrawLine(vgaNextScreenBuff,pts[i].x,pts[i].y,pts[i+1].x,pts[i+1].y);
}
#endif
