#pragma once

/* const */
const float pi = 3.14159265359;

/* Structs */
typedef struct{/* Point */
	float x;
	float y;
} Pt_t;	


/* Functions */
void drawTime(int x, int y, bool small);
void drawEllipse(int x, int y,int ra, int rb);
void drawSmallClock(int x, int y);
void drawShape(Pt_t *pts, int size);
Pt_t applyRotation(Pt_t pt, float th);