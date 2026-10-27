// FUNC 80021b20 1016 MAIN0
// MATCHING 80021b20 1016
/* smax: asm("" : "=r"(r) : "0"(r)) after r = a gives r the a0 priority; smin takes int a (lhu + off) with r pinned
   a0 and the caller's << 16 through a v0-pinned temp; q row loaded before TASK_C->w4c, then q += D982 * 4;
   asm("") after the 0x1F8000F2 load keeps the w30 load after it; p34 pinned a0 in the second clamp. */
#include "spad.h"
#include "gcd.h"
typedef struct { short x, y; } SP;
typedef struct { short a, b, c, d; } S4;
typedef struct { char p[0x4c]; unsigned short w4c; } O;
#include "camera.h"
extern unsigned short GAME[];
extern unsigned short GAME_SELECTED_SECTION;
extern unsigned short GAME_SELECTED_SPAWN_POINT;
extern S4 *D_80078B7C[];
extern char D_800792BC[];
extern char D_800792D4[];
extern char D_800792E8[];
extern char D_80079300[];
extern char D_80079318[];
extern char D_80079368[];
extern char D_80079378[];
extern char D_80079384[];
extern char D_80079394[];
extern char D_800793B0[];
extern char D_800793D4[];
extern char D_800793EC[];
extern char D_800793F4[];
extern char D_80079414[];
extern char D_80079430[];
extern char D_8007945C[];
extern char D_80079468[];
static short **D_80079474[24] = {
    (short **)D_800792BC, (short **)D_800792D4, (short **)D_800792E8, (short **)D_80079300,
    (short **)D_80079318, (short **)D_80079368, (short **)D_80079378, (short **)D_800792D4,
    (short **)D_80079384, (short **)D_80079394, (short **)D_800793B0, (short **)D_800793D4,
    (short **)D_80079318, (short **)D_800793EC, (short **)D_800793F4, (short **)D_800793F4,
    (short **)D_80079414, (short **)D_80079430, (short **)D_8007945C, (short **)D_80079468,
    (short **)D_80079430, (short **)D_8007945C, (short **)D_80079468, (short **)D_800792BC,
};
extern short D_800A457C, D_800A457E, D_800A4580, D_800A4582;
extern int *D_800A4584, *D_800A4588;
extern O *TASK_C;
extern void computeCameraMatrix(CameraScroll *);
#define FLAG (*(unsigned short *)0x1F8001C8)
#define SFLAG (*(short *)0x1F8001C8)

static __inline__ int smax(int a, short b) { short r = a; asm("" : "=r"(r) : "0"(r)); if (a < b) r = b; return r; }
static __inline__ int smin(int a, short b) { register short r asm("$4") = a; if ((short)a > b) r = b; return r; }

void initSectionCamera(void)
{
    CameraScroll *c;
    short *p, *q;
    int off, x, v;
    unsigned short w;
    char pad[0x10];
    c = &CAMERA_SCROLL;
    c->active = 1;
    p = (short *)&D_80078B7C[GAME[0]][GAME_SELECTED_SECTION];
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
    if ((GAME[0] == 4 && GAME_SELECTED_SECTION < 4) || *(int *)GAME == 0x50000)
        ((int*)&SPAD.ie0)[2] = 0xFDC00000;
    else
        ((int*)&SPAD.ie0)[2] = 0xFDE00000;
    q = D_80079474[GAME[0]][GAME_SELECTED_SECTION];
    w = TASK_C->w4c;
    q += GAME_SELECTED_SPAWN_POINT * 4;
    switch (w) {
    case 1: case 2:
        off = -0x38;
        break;
    case 4: case 5: case 6:
        off = 0;
        break;
    }
    if (!(FLAG & 1)) {
        ((int*)&SPAD.ie0)[3] = smax(*q++, c->minX) << 16;
        { register int t asm("$2"); t = smin((unsigned short)*q++ + off, c->maxY) << 16; ((int*)&SPAD.ie0)[4] = t; }
        ((int*)&SPAD.ie0)[5] = *q << 16;
    } else {
        ((int*)&SPAD.ie0)[3] = *q++ << 16;
        { register int t asm("$2"); t = smin((unsigned short)*q++ + off, c->maxY) << 16; ((int*)&SPAD.ie0)[4] = t; }
        ((int*)&SPAD.ie0)[5] = smax(*q, c->minX) << 16;
    }
    x = *(short *)0x1F8000F2;
    asm("");
    v = c->minY;
    if (x < v || (v = c->maxY, v < x))
        ((int*)&SPAD.ie0)[4] = v << 16;
    { register int *pp asm("$4") = c->targetX;
    v = c->maxX;
    if (v < ((SP *)pp)->y || (v = c->minX, ((SP *)pp)->y < v))
        *pp = v << 16; }
    if (GCD.bcda2) {
        c->rotX = 0x1000;
        if (FLAG & 1) c->rotY = -0x1000;
        else c->rotY = 0x1000;
        c->rotXOffset = 0;
        c->rotYOffset = SFLAG * 0x5A00;
    } else {
        c->rotX = 0;
        c->rotY = 0;
        c->rotXOffset = 0;
        c->rotYOffset = SFLAG * 0x5A00;
    }
    computeCameraMatrix(c);
    ((int*)&SPAD.i174)[0] = c->pos.x;
    ((int*)&SPAD.i174)[1] = c->pos.y;
    ((int*)&SPAD.i174)[2] = c->pos.z;
    ((int*)&SPAD.i174)[3] = c->pos.x;
    ((int*)&SPAD.i174)[4] = c->pos.y + ((int*)&SPAD.ie0)[1];
    ((int*)&SPAD.i174)[5] = c->pos.z;
}
