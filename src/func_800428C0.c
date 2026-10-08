// FUNC 800428c0 272 MAIN0
// MATCHING 800428c0 272
// tamano real 272 (Ghidra corta en 188). Con register asm("$17") da MATCH; sin el, r/o intercambian s0/s1.
#include "TOBJ.H"
extern int func_800425C4(TObj *a, char *b);
extern void FUN_8001f96c(int a, int b, int c, int d);
extern void playSFX(int a);

int func_800428C0(TObj *o, char *p)
{
    register int r asm("$17");
    r = 1;
    p[0x68] = 1;
    p[0x9e] = 0;
    switch (func_800425C4(o, p)) {
    case 1: case 7:
        r = 1;
        break;
    case 4: case 5: case 10:
        r = 1;
        goto L;
    case 0: case 3: case 9:
        r = 2;
        break;
    case 13:
        p[0x68] = 0;
        r = -1;
        break;
    case 6: case 11: case 12:
        r = 2;
    L:
        o->b6a = 1;
        o->active = 2;
        o->wa8 = 0x4ff;
        break;
    case 2: case 8:
        r = 0;
        p[0x9e] = 1;
        p[0x9f] = 0;
        break;
    }
    if (r < 0) {
        r = 0;
    } else {
        int s;
        FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        s = 7;
        if ((p[0x1c] & 0x7f) == 4) s = 6;
        playSFX(s);
    }
    return r;
}
