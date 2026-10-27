// FUNC 80122724 408 X000
// MATCHING 80122724 408
// VERSION SLPM_80165 x00 80119B00 396
// volatile b/visible stores keep the game order (matching debt)
// Tried: no volatile with every order of the a/y/b stores, b inside/after the `one` block, short/uchar one, FUN(o, o->visible = 1): sb stays above sh zero (2) or two li (25).
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 80122724 (func_80122724.c):  Starts at its placed height (PAL +0x180).
// volatile b/visible stores keep the game order (matching debt)
// Tried: no volatile with every order of the a/y/b stores, b inside/after the `one` block, short/uchar one, FUN(o, o->visible = 1): sb stays above sh zero (2) or two li (25).
#include "TOBJ.H"
extern void *A00_FAR_BACKDROP_EAST_ANIMS[];
extern int DAT_1f8002d4[];
extern short SCREEN_ORIGIN_X;
extern unsigned short SCREEN_ORIGIN_Y;
extern void FUN_80018da4(TObj *, int);
extern void freeObjectLayer7(TObj *);

void func_80122724(TObj *o)
{
    unsigned short v, t;
    short r;

    switch (o->b04) {
    case 0:
        o->w1e = 0xc;
        o->b0d = 0;
        o->anim = A00_FAR_BACKDROP_EAST_ANIMS[o->b0c];
        o->d3c = DAT_1f8002d4[0];
        o->b0f = 0;
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->b04++;
        break;
    case 1:
        r = SCREEN_ORIGIN_X;
        v = r;
        if (r >= 0x719) {
            if (o->b0c) {
                v = (short)(o->d30 - v) >> 3;
            } else {
                t = o->d30 - v;
                if ((short)t < -0x200)
                    v = ((unsigned)t & 0xfff) >> 3;
                else
                    v = (short)t >> 3;
                if ((short)v >= 0x180)
                    break;
            }
            o->a.p.whole = v;
            o->y.p.whole = ((unsigned)(o->d34 - SCREEN_ORIGIN_Y) & 0xfff) >> 4;
            *(volatile short *)&o->b.p.whole = 0;
            { int one = 1; *(volatile unsigned char *)&o->visible = one; FUN_80018da4(o, one); }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer7(o);
        break;
    }
}
#else
#include "TOBJ.H"
extern void *A00_FAR_BACKDROP_EAST_ANIMS[];
extern int DAT_1f8002d4[];
extern short SCREEN_ORIGIN_X;
extern unsigned short SCREEN_ORIGIN_Y;
extern void FUN_80018da4(TObj *, int);
extern void freeObjectLayer7(TObj *);

void updateA00FarBackdropEast(TObj *o)
{
    unsigned short v, t;
    short r;

    switch (o->b04) {
    case 0:
        o->w1e = 0xc;
        o->b0d = 0;
        o->y.p.whole += 0x180;
        o->anim = A00_FAR_BACKDROP_EAST_ANIMS[o->b0c];
        o->d3c = DAT_1f8002d4[0];
        o->b0f = 0;
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->b04++;
        break;
    case 1:
        r = SCREEN_ORIGIN_X;
        v = r;
        if (r >= 0x719) {
            if (o->b0c) {
                v = (short)(o->d30 - v) >> 3;
            } else {
                t = o->d30 - v;
                if ((short)t < -0x200)
                    v = ((unsigned)t & 0xfff) >> 3;
                else
                    v = (short)t >> 3;
                if ((short)v >= 0x180)
                    break;
            }
            o->a.p.whole = v;
            o->y.p.whole = ((unsigned)(o->d34 - SCREEN_ORIGIN_Y) & 0xfff) >> 4;
            *(volatile short *)&o->b.p.whole = 0;
            { int one = 1; *(volatile unsigned char *)&o->visible = one; FUN_80018da4(o, one); }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer7(o);
        break;
    }
}
#endif
