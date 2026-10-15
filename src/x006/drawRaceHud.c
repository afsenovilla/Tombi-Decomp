// FUNC 8011bcac 148 X006
// MATCHING 8011bcac 148
#include "spad.h"
typedef struct { char p0[0x14]; unsigned short w14; char p16[0x60 - 0x16]; } L;
typedef struct { char p0[0x18]; int f18; } P;
extern void FUN_8005f990(L *);
extern void AddDrawMode(int, int);
extern void drawRaceHudBestTime(P *, int, int);
extern void drawRaceHudLapTime(P *, int, int);
extern void drawRaceHudGauge(P *, int, int, int);
extern void drawRaceHudLives(P *, int, int);

void drawRaceHud(P *p)
{
    L l;
    FUN_8005f990(&l);
    AddDrawMode(l.w14, 1);
    p->f18 = SPAD.i2d4;
    drawRaceHudBestTime(p, 0x10, 0x10);
    drawRaceHudLapTime(p, 0x10, 0x1a);
    drawRaceHudGauge(p, 0, 0x88, 0xf0);
    p->f18 = SPAD.i2d8;
    drawRaceHudLives(p, 0x18, 0xf0);
}
