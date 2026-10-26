// FUNC 801176d8 1492 X000
// MATCHING 801176d8 1492
#include "camera.h"
#include "spad.h"
#include "gcd.h"
/* Spans splat entries 801176D8+80117928 (jtbl 80115EF8). Each branch computes lo = x / 80 and
   hi = lo + N itself so the shared tail is cross-jumped; x pinned to v1 ($2 for the D_1F800176u load). */
typedef struct { char pad[3]; unsigned char n; unsigned char *p[1]; } L;
extern unsigned short D_8009C962;
extern unsigned short D_1F800176u;
extern int getScrollOffsetX(void);

void a00LinkAreaModelBlocks(L *o, unsigned char *base)
{
    unsigned char cnt = base[0];
    int *tbl = (int *)(base + 4);
    short hi, i;
    short lo;
    register int x asm("$3");
    register int x2 asm("$2");

    switch (D_8009C962) {
    case 0:
        if (GCD.bcda2)
            { x = (*(short*)((char*)&SPAD.i174 + 2)) + 0x8c; lo = x / 80; hi = lo + 8; }
        else if (CAMERA_SCROLL.rotY == 0)
            { x = (*(short*)((char*)&SPAD.i174 + 2)) + 0x64; lo = x / 80; hi = lo + 8; }
        else
            { x = (*(short*)((char*)&SPAD.i174 + 2)) + getScrollOffsetX() + 0x64; lo = x / 80; hi = lo + 8; }
        if (lo <= 0)
            lo = 1;
        if (hi >= cnt)
            hi = cnt - 1;
        for (i = lo; i <= hi; i++) {
            o->p[o->n] = base + tbl[i];
            o->n++;
        }
        break;
    case 1:
        if (GCD.bcda2)
            { x = (*(short*)((char*)&SPAD.i174 + 2)) + 0x3c; lo = x / 80; hi = lo + 9; }
        else if (CAMERA_SCROLL.rotY == 0)
            { x = (*(short*)((char*)&SPAD.i174 + 2)) + 0x3c; lo = x / 80; hi = lo + 9; }
        else
            { x = (*(short*)((char*)&SPAD.i174 + 2)) + getScrollOffsetX() + 0x3c; lo = x / 80; hi = lo + 9; }
        if (lo <= 0)
            lo = 1;
        if (hi >= cnt)
            hi = cnt - 1;
        for (i = lo; i <= hi; i++) {
            o->p[o->n] = base + tbl[i];
            o->n++;
        }
        break;
    case 2:
        if (CAMERA_SCROLL.rotY == 0)
            { x = (*(short*)((char*)&SPAD.i174 + 2)) + 0x8c; lo = x / 80; hi = lo + 8; }
        else
            { x = (*(short*)((char*)&SPAD.i174 + 2)) + getScrollOffsetX() + 0x8c; lo = x / 80; hi = lo + 8; }
        if (lo <= 0)
            lo = 1;
        if (hi >= 42)
            hi = 41;
        for (i = lo; i <= hi; i++) {
            o->p[o->n] = base + tbl[i];
            o->n++;
        }
        lo = ((*(short*)&SPAD.s186) + 0x550) / 80;
        if (lo < 0)
            lo = 0;
        lo += 42;
        hi = lo + 7;
        if (hi >= cnt)
            hi = cnt - 1;
        for (i = lo; i <= hi; i++) {
            o->p[o->n] = base + tbl[i];
            o->n++;
        }
        break;
    case 4:
    case 5:
        if (GCD.bcda2) {
            for (i = 0; i < cnt; i++) {
                o->p[i] = base + tbl[i];
                o->n++;
            }
        } else {
            if (CAMERA_SCROLL.rotY == 0)
                { x2 = (short)D_1F800176u; lo = x2 / 80; hi = lo + 8; }
            else
                { x = (*(short*)((char*)&SPAD.i174 + 2)) + getScrollOffsetX(); lo = x / 80; hi = lo + 8; }
        }
        if (lo < 0)
            lo = 0;
        if (hi >= cnt)
            hi = cnt - 1;
        for (i = lo; i <= hi; i++) {
            o->p[o->n] = base + tbl[i];
            o->n++;
        }
        break;
    }
}
