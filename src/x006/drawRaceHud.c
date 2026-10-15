// FUNC 8011bcac 148 X006
// MATCHING 8011bcac 148
// VERSION SLPS_01144 x06 8011AE2C 148
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x006 8011BCAC (x006/func_8011BCAC.c): 240 -> 208.
#include "spad.h"
typedef struct { char p0[0x14]; unsigned short w14; char p16[0x60 - 0x16]; } L;
typedef struct { char p0[0x18]; int f18; } P;
extern void FUN_8005f990(L *);
extern void AddDrawMode(int, int);
extern void func_8011BD40(P *, int, int);
extern void func_8011C4AC(P *, int, int);
extern void func_8011CC18(P *, int, int, int);
extern void func_8011CD78(P *, int, int);

void func_8011BCAC(P *p)
{
    L l;
    FUN_8005f990(&l);
    AddDrawMode(l.w14, 1);
    p->f18 = SPAD.i2d4;
    func_8011BD40(p, 0x10, 0x10);
    func_8011C4AC(p, 0x10, 0x1a);
    func_8011CC18(p, 0, 0x88, 0xd0);
    p->f18 = SPAD.i2d8;
    func_8011CD78(p, 0x18, 0xd0);
}
#else
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
#endif
