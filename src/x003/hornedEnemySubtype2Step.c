// FUNC 80126f38 788 X003
// MATCHING 80126f38 788
// VERSION SLPS_01144 x03 80125D9C 788
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x003 80126F38 (x003/func_80126F38.c): 160 -> 152.
#include "spad.h"
#include "TOBJ.H"

extern char D_80077D3C[];
extern void *D_80139510;
extern unsigned char D_80135CB0[];
extern short D_800A4582;
extern void playSFX(int);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void applyFrameVelocityX(TObj *);
extern short FUN_8004065c(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);

void func_80126F38(TObj *o)
{
    unsigned char *b;
    int v;

    switch (o->state) {
    case 0:
        o->b9c = 1;
        o->movetab = D_80077D3C;
        o->wac = 4;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->state++;
        o->anim = D_80139510;
        b = &D_80135CB0[o->wac * 4];
        o->box0 = *b++;
        o->box1 = *b++;
        o->box2 = *b++;
        o->box3 = *b++;
        AnimLoadDuration(o);
    case 1:
        if (o->visible && ((SPAD.s1f8 + SPAD.i198) & 0x1f) == 0) playSFX(0x76);
        AnimAdvance(o);
        applyFrameVelocityX(o);
        if (o->animFrame & 1)
            v = -0x18;
        else
            v = 0x18;
        if (FUN_8004065c(o, o->h->p.whole + v, o->y.p.whole, o->animFrame)
            || TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0xe)
            || (unsigned short)(v = (unsigned short)(*(short*)((char*)&SPAD.i168 + 2)) - (unsigned short)o->h->p.whole + 0x40) < 0x80) {
            o->b9c = 2;
            o->state++;
        }
        break;
    case 2:
        if (o->visible && ((SPAD.s1f8 + SPAD.i198) & 0x1f) == 0) playSFX(0x76);
        AnimAdvance(o);
        o->y.p.whole++;
        if (o->animFrame & 1)
            v = -0x18;
        else
            v = 0x18;
        FUN_8004065c(o, o->h->p.whole + v, o->y.p.whole, o->animFrame);
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0xe)) {
            o->d8c = (-SPAD.s27e << 2) & 0xff;
            o->state = 1;
            o->b9c = 0;
            o->step++;
        }
        break;
    }
    if (((SPAD.s1f8 + SPAD.i198) & 0xf) == 0 && D_800A4582 + 0x98 < o->y.p.whole) o->b04 = 3;
}
#else
#include "spad.h"
#include "TOBJ.H"

extern char D_80077D3C[];
extern void *D_80139510;
extern unsigned char D_80135CB0[];
extern short D_800A4582;
extern void playSFX(int);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void applyFrameVelocityX(TObj *);
extern short probeWallAt(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);

void hornedEnemySubtype2Step(TObj *o)
{
    unsigned char *b;
    int v;

    switch (o->state) {
    case 0:
        o->b9c = 1;
        o->movetab = D_80077D3C;
        o->wac = 4;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->state++;
        o->anim = D_80139510;
        b = &D_80135CB0[o->wac * 4];
        o->box0 = *b++;
        o->box1 = *b++;
        o->box2 = *b++;
        o->box3 = *b++;
        AnimLoadDuration(o);
    case 1:
        if (o->visible && ((SPAD.s1f8 + SPAD.i198) & 0x1f) == 0) playSFX(0x76);
        AnimAdvance(o);
        applyFrameVelocityX(o);
        if (o->animFrame & 1)
            v = -0x18;
        else
            v = 0x18;
        if (probeWallAt(o, o->h->p.whole + v, o->y.p.whole, o->animFrame)
            || TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0xe)
            || (unsigned short)(v = (unsigned short)(*(short*)((char*)&SPAD.i168 + 2)) - (unsigned short)o->h->p.whole + 0x40) < 0x80) {
            o->b9c = 2;
            o->state++;
        }
        break;
    case 2:
        if (o->visible && ((SPAD.s1f8 + SPAD.i198) & 0x1f) == 0) playSFX(0x76);
        AnimAdvance(o);
        o->y.p.whole++;
        if (o->animFrame & 1)
            v = -0x18;
        else
            v = 0x18;
        probeWallAt(o, o->h->p.whole + v, o->y.p.whole, o->animFrame);
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0xe)) {
            o->d8c = (-SPAD.s27e << 2) & 0xff;
            o->state = 1;
            o->b9c = 0;
            o->step++;
        }
        break;
    }
    if (((SPAD.s1f8 + SPAD.i198) & 0xf) == 0 && D_800A4582 + 0xa0 < o->y.p.whole) o->b04 = 3;
}
#endif
