#include "draw.h"
extern "C"{
#include "drivers.h"
#include "vga.h"
#include "arm_math.h"
}
#include <math.h>

/* next screen buffer */
extern uint8_t* vgaNextScreenBuffR;
extern uint8_t* vgaNextScreenBuffG;
extern uint8_t* vgaNextScreenBuffB;
/* functions */
Pt_t applyRotation(Pt_t pt, float th){
	/* Apply rotation */
	Pt_t ret;
	ret.x = int(float(pt.x) * cos(th) + float(pt.y) * sin(th));
	ret.y = int(float(pt.y) * cos(th) - float(pt.x) * sin(th));
	return ret;
}

void drawShape(Pt_t *pts, int size){
	for(int i=0;i<(size-1);i++){
		VGADrawLine(vgaNextScreenBuffR,pts[i].x,pts[i].y,pts[i+1].x,pts[i+1].y);
		VGADrawLine(vgaNextScreenBuffG,pts[i].x,pts[i].y,pts[i+1].x,pts[i+1].y);
		VGADrawLine(vgaNextScreenBuffB,pts[i].x,pts[i].y,pts[i+1].x,pts[i+1].y);
	}	
}
void drawTime(int x, int y, bool small){
	char timeChar[6] = {};
	getTimeBcd(timeChar);
	int lasti = 8;
	if(small)
		lasti = 5;
	int j = 0;
	for(int i=0;i<lasti;i++){
		if(i==2 || i==5){
			//VGAPutChar(vgaNextScreenBuffR,x+8*i,y,':');
			VGAPutChar(vgaNextScreenBuffG,x+8*i,y,':');
			VGAPutChar(vgaNextScreenBuffB,x+8*i,y,':');
		}
		else{
			//VGAPutChar(vgaNextScreenBuffR,x+8*i,y,timeChar[j]+'0');
			VGAPutChar(vgaNextScreenBuffG,x+8*i,y,timeChar[j]+'0');
			VGAPutChar(vgaNextScreenBuffB,x+8*i,y,timeChar[j]+'0');
			j++;
		}
	}
}

void drawEllipse(int X, int Y,int RA, int RB){
	float ra = float(RA);
	float rb = float(RB);
	float dt = (ra+rb)*pi/(16.0*ra*rb);
	for(float t = 0; t < pi; t+=dt){
		float x = ra*arm_cos_f32(t);
		float y = rb*arm_sin_f32(t);
		VGApx(vgaNextScreenBuffR,int(x)+X,Y+int(y),SET);
		VGApx(vgaNextScreenBuffR,int(x)+X,Y-int(y),SET);
		VGApx(vgaNextScreenBuffG,int(x)+X,Y+int(y),SET);
		VGApx(vgaNextScreenBuffG,int(x)+X,Y-int(y),SET);
		VGApx(vgaNextScreenBuffB,int(x)+X,Y+int(y),SET);
		VGApx(vgaNextScreenBuffB,int(x)+X,Y-int(y),SET);
	}
}
void drawSmallClock(int x, int y){
	const int r = 24;
	/* small time */
	drawTime(x-19, y-4, true);
	
	/* get time */
	int time[3] = {};
	getTimeNum(time);
	
	/* frame */
	//VGADrawCircle(vgaNextScreenBuff,x,y,r,0);
	drawEllipse(x,y,r,r);
	#if 0
	/* aiguilles */
	float fr = float(r);
	/* hours */
	Pt_t hour;
	float arg = pi * float(time[0]) / 6.0;
	float frMod = fr*1.1;
	hour.x = frMod*sin(arg) + float(x);
	hour.y = -frMod*cos(arg) + float(y);
	VGApx(vgaNextScreenBuff,int(hour.x),int(hour.y),INV);
	/* minutes */
	Pt_t min;
	arg = pi * float(time[1]) / 30.0;
	frMod = fr*0.9;
	min.x = frMod*sin(arg) + float(x);
	min.y = -frMod*cos(arg) + float(y);
	VGApx(vgaNextScreenBuff,int(min.x),int(min.y),INV);
	
	/* seconds */
	Pt_t sec;
	arg = pi * float(time[2]) / 30.0;
	sec.x = frMod*sin(arg) + float(x);
	sec.y = -frMod*cos(arg) + float(y);
	VGApx(vgaNextScreenBuff,int(sec.x),int(sec.y),INV);
	#endif
}