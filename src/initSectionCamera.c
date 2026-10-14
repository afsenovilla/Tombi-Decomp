// FUNC 80021b20 1016 MAIN0
// MATCHING 80021b20 1016
/* smax: asm("" : "=r"(r) : "0"(r)) after r = a gives r the a0 priority; smin takes int a (lhu + off) with r pinned
   a0 and the caller's << 16 through a v0-pinned temp; q row loaded before TASK_C->w4c, then q += D982 * 4;
   asm("") after the 0x1F8000F2 load keeps the w30 load after it; p34 pinned a0 in the second clamp. */
#include "spad.h"
#include "gcd.h"
typedef struct { int x, y, z; } V3;
typedef struct { short x, y; } SP;
typedef struct { short a, b, c, d; } S4;
typedef struct {
    unsigned char b0; char p1[7]; V3 pos; char p14[0x20 - 0x14]; int d20; int d24;
    char p28[0x2c - 0x28]; short w2c, w2e, w30, w32; int *p34; char p38[0x78 - 0x38]; int d78; int d7c;
} C;
typedef struct { char p[0x4c]; unsigned short w4c; } O;
extern C D_800A4550;
extern unsigned short D_8009C960[];
extern unsigned short D_8009C962;
extern unsigned short D_8009C982;
extern S4 *D_80078B7C[];
extern short **D_80079474[];
extern short D_800A457C, D_800A457E, D_800A4580, D_800A4582;
extern int *D_800A4584, *D_800A4588;
extern O *TASK_C;
extern void computeCameraMatrix(C *);
#define FLAG (*(unsigned short *)0x1F8001C8)
#define SFLAG (*(short *)0x1F8001C8)

static __inline__ int smax(int a, short b) { short r = a; asm("" : "=r"(r) : "0"(r)); if (a < b) r = b; return r; }
static __inline__ int smin(int a, short b) { register short r asm("$4") = a; if ((short)a > b) r = b; return r; }

void initSectionCamera(void)
{
    C *c;
    short *p, *q;
    int off, x, v;
    unsigned short w;
    char pad[0x10];
    c = &D_800A4550;
    c->b0 = 1;
    p = (short *)&D_80078B7C[D_8009C960[0]][D_8009C962];
    D_800A457C = *p++;
    D_800A457E = *p++;
    D_800A4580 = *p++;
    D_800A4582 = *p;
    if (!(FLAG & 1)) {
        D_800A4584 = &((int*)&SPAD.ie0)[3];
        D_800A4588 = &((int*)&SPAD.ie0)[5];
    } else {
        D_800A4588 = &((int*)&SPAD.ie0)[3];
        D_800A4584 = &((int*)&SPAD.ie0)[5];
    }
    ((int*)&SPAD.ie0)[0] = 0;
    ((int*)&SPAD.ie0)[1] = 0;
    if ((D_8009C960[0] == 4 && D_8009C962 < 4) || *(int *)D_8009C960 == 0x50000)
        ((int*)&SPAD.ie0)[2] = 0xFDC00000;
    else
        ((int*)&SPAD.ie0)[2] = 0xFDE00000;
    q = D_80079474[D_8009C960[0]][D_8009C962];
    w = TASK_C->w4c;
    q += D_8009C982 * 4;
    switch (w) {
    case 1: case 2:
        off = -0x38;
        break;
    case 4: case 5: case 6:
        off = 0;
        break;
    }
    if (!(FLAG & 1)) {
        ((int*)&SPAD.ie0)[3] = smax(*q++, c->w2c) << 16;
        { register int t asm("$2"); t = smin((unsigned short)*q++ + off, c->w32) << 16; ((int*)&SPAD.ie0)[4] = t; }
        ((int*)&SPAD.ie0)[5] = *q << 16;
    } else {
        ((int*)&SPAD.ie0)[3] = *q++ << 16;
        { register int t asm("$2"); t = smin((unsigned short)*q++ + off, c->w32) << 16; ((int*)&SPAD.ie0)[4] = t; }
        ((int*)&SPAD.ie0)[5] = smax(*q, c->w2c) << 16;
    }
    x = *(short *)0x1F8000F2;
    asm("");
    v = c->w30;
    if (x < v || (v = c->w32, v < x))
        ((int*)&SPAD.ie0)[4] = v << 16;
    { register int *pp asm("$4") = c->p34;
    v = c->w2e;
    if (v < ((SP *)pp)->y || (v = c->w2c, ((SP *)pp)->y < v))
        *pp = v << 16; }
    if (GCD.bcda2) {
        c->d20 = 0x1000;
        if (FLAG & 1) c->d24 = -0x1000;
        else c->d24 = 0x1000;
        c->d78 = 0;
        c->d7c = SFLAG * 0x5A00;
    } else {
        c->d20 = 0;
        c->d24 = 0;
        c->d78 = 0;
        c->d7c = SFLAG * 0x5A00;
    }
    computeCameraMatrix(c);
    ((int*)&SPAD.i174)[0] = c->pos.x;
    ((int*)&SPAD.i174)[1] = c->pos.y;
    ((int*)&SPAD.i174)[2] = c->pos.z;
    ((int*)&SPAD.i174)[3] = c->pos.x;
    ((int*)&SPAD.i174)[4] = c->pos.y + ((int*)&SPAD.ie0)[1];
    ((int*)&SPAD.i174)[5] = c->pos.z;
}
