// FUNC 8003509c 128 MAIN0
// MATCHING 8003509c 128
// VERSION SLPS_01144 slps01144 80035FC4 128
// FLAGS -O1 -G0
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("slps01144/nonmatchings/slps01144", func_80035FC4_slps01144);
#else
#include "TOBJ.H"
extern volatile int DAT_1f800340[];
extern unsigned char PLAYER_DRAW_DEPTH;
extern unsigned short PLAYER_X, PLAYER_Y, DAT_800a6052;
void weapon5to7Init(TObj *o, int n)
{
    volatile int *p;
    int q, a, b;
    unsigned char c;
    unsigned short s;
    p = DAT_1f800340;
    n <<= 2;
    q = p[0];
    a = p[0];
    b = *(int *)(q + n + 4);
    o->ba4 = 1;
    o->da0 = a + b;
    c = PLAYER_DRAW_DEPTH;
    o->b0a = 0x11;
    o->b0f = c - 2;
    o->a.p.whole = PLAYER_X;
    o->y.p.whole = PLAYER_Y;
    s = DAT_800a6052;
    o->d84 = 0;
    o->d88 = 0;
    o->d8c = 0;
    o->b.p.whole = s;
}
#endif
