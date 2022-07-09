#include "draw.h"
extern "C"{
#include "drivers.h"
#include "vga.h"
}
#include <math.h>

/* next screen buffer */
extern uint8_t* vgaNextScreenBuff;

/* functions */
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

void drawTime(int x, int y, bool small){
	char timeChar[6] = {};
	getTimeBcd(timeChar);
	int lasti = 8;
	if(small)
		lasti = 5;
	int j = 0;
	for(int i=0;i<lasti;i++){
		if(i==2 || i==5)
			VGAPutChar(vgaNextScreenBuff,x+8*i,y,':');
		else{
			VGAPutChar(vgaNextScreenBuff,x+8*i,y,timeChar[j]+'0');
			j++;
		}
	}
}

void drawEllipse(int X, int Y,int RA, int RB){
	float ra = float(RA);
	float rb = float(RB);
	for(int xi = -RA; xi <= RA; xi++){
		float x = float(xi);
		float y1 = rb*sqrt(1-(x*x/(ra*ra)))+float(Y);
		float y2 = -rb*sqrt(1-(x*x/(ra*ra)))+float(Y);
		
		VGApx(vgaNextScreenBuff,xi+X,int(y1));
		VGApx(vgaNextScreenBuff,xi+X,int(y2));
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
	VGADrawCircle(vgaNextScreenBuff,x,y,r,0);
	//drawEllipse(x,y,r,8);
		
	/* aiguilles */
	float fr = float(r);
	/* hours */
	Pt_t hour;
	float arg = pi * float(time[0]) / 6.0;
	float frMod = fr*1.1;
	hour.x = frMod*sin(arg) + float(x);
	hour.y = -frMod*cos(arg) + float(y);
	VGApx(vgaNextScreenBuff,int(hour.x),int(hour.y));
	/* minutes */
	Pt_t min;
	arg = pi * float(time[1]) / 30.0;
	frMod = fr*0.9;
	min.x = frMod*sin(arg) + float(x);
	min.y = -frMod*cos(arg) + float(y);
	VGApx(vgaNextScreenBuff,int(min.x),int(min.y));
	
	/* seconds */
	Pt_t sec;
	arg = pi * float(time[2]) / 30.0;
	sec.x = frMod*sin(arg) + float(x);
	sec.y = -frMod*cos(arg) + float(y);
	VGApx(vgaNextScreenBuff,int(sec.x),int(sec.y));
		
}