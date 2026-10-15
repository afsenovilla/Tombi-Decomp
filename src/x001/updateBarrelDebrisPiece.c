// FUNC 80132760 516 X001
// MATCHING 80132760 516
#include "spad.h"
#include "TOBJ.H"
typedef struct { short a, b; } S2;
extern S2 D_8013C9C0[];
extern char D_8013EB48[];
extern char D_8013EB50[];
extern char D_8013EB58[];
extern char D_8013EB60[];
extern char D_8013EB68[];
extern char D_8013EB70[];
extern char D_8013EB78[];
static void *D_8013E6E4[7] = {
    D_8013EB48, D_8013EB50, D_8013EB58, D_8013EB60,
    D_8013EB68, D_8013EB70, D_8013EB78,
};
extern char D_80077CF4[];
extern void AnimLoadDuration(TObj *);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern int FUN_800202b4(TObj *);
extern void FUN_800187e4(TObj *);

void updateBarrelDebrisPiece(TObj *o)
{
    unsigned char k;

    switch (o->b04) {
    case 0:
        o->b04++;
        k = o->subtype;
        *(signed char *)&o->b0f = -20;
        o->b0a = 2;
        o->timer = 0x3c;
        o->movetab = D_80077CF4;
        o->d8c = 0;
        o->b0d = 0;
        o->animFrame = 0;
        { S2 *p = &D_8013C9C0[k]; o->w22 = p->a; o->velV = p->b; }
        o->w1e = 10;
        o->d3c = ((int*)&SPAD.i2d4)[0];
        k = o->subtype;
        o->anim = D_8013E6E4[k];
        AnimLoadDuration(o);
        break;
    case 1:
        o->velV += 0x28;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        if (o->w22 & 1) o->d8c = (unsigned char)(o->d8c + 8);
        else o->d8c = (unsigned char)(o->d8c - 8);
        if (o->w22 != 2) applyAnimVelocityX(o, o->w22);
        if (o->timer >= 0x1e || ((SPAD.s1f8 + SPAD.i198) & 1)) FUN_800202b4(o);
        if (--o->timer == -1) o->b04 = 2;
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
