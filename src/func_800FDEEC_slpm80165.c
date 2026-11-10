// VERSION SLPM_80165 x00 800FDEEC 288
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_80104AA4_slps01144 (src/playerBittenCarryAirStep.c) with setFacingFromPad inline (d-pad
// bits shifted left by 8).
#include "TOBJ.H"
extern TObj *BITTEN_OBJECT;
extern volatile unsigned short PAD_PRESSED;
static __inline__ void demoSetFacingFromPad(void *o)
{
    unsigned short v = *(unsigned short *)((char *)o + 0x2e) & 1;
    volatile unsigned short *p = &PAD_PRESSED;
    *(unsigned short *)((char *)o + 0x2e) = v;
    if (*p & 0x2000) {
        *(short *)((char *)o + 0x2e) = 0;
    } else {
        *(unsigned short *)((char *)o + 0x2e) = (*p & 0x8000) ? 1 : (v | 2);
    }
}
extern void FUN_8010f0f4(TObj *);
extern void FUN_8001fd94(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_80040278(TObj *, int, int);
extern int FUN_8003facc(TObj *);

void playerBittenCarryAirStep(TObj *o)
{
    TObj *p = BITTEN_OBJECT;

    p->h->p.whole = o->h->p.whole;
    p->y.p.whole = o->y.p.whole + p->box2;
    demoSetFacingFromPad(o);
    FUN_8010f0f4(o);
    FUN_8001fd94(o);
    FUN_8001fec0(o);
    if (o->velY > 0) {
        o->b9c = 2;
        o->d84 = 0;
        o->velY = 0;
        o->substep = 1;
    }
    FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
    if (FUN_8003facc(o) != 0) {
        o->b9c = 2;
        o->d84 = 0;
        o->velY = 0;
        o->substep = 1;
    }
}
#endif
