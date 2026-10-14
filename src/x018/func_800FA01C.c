// FUNC 800fa01c 832 X018
// MATCHING 800fa01c 832
#include "plx.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
extern TObj *D_8009C330;
extern unsigned char *D_8009D2E8;
extern unsigned short D_1f8001fc, D_1f8003c6;
extern void ObjSetAnimFromTable(TObj *);
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern void SfxPlay3(int, int);
extern void probeCollisionAtDepthC(TObj *, int, int);
extern void FUN_800ef1ac(TObj *);

static __inline__ void setanim(TObj *o, unsigned short anim)
{
    TObj *p = D_8009C330;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        ObjSetAnimFromTable(o);
        AnimJump(o, 0);
        D_8009C330->animFrame = D_8009C330->animTimer;
    }
}

typedef struct { unsigned char b0; char p1; short w2; char p4[8]; short wc; short we; char p10[0x28 - 0x10]; unsigned short w28, w2a; } PL;

void func_800FA01C(TObj *o)
{
    PL *p;
    switch (o->state) {
    case 0:
        p = (PL *)D_8009C330;
        o->wb6 = 0;
        U8(p, 8) = 0;
        p->we = 0;
        p->w2 = 0;
        (*(unsigned char*)((char*)&D_8009C330->w08 + 1)) = 0;
        ((PL *)D_8009C330)->wc = 0;
        ((PL *)D_8009C330)->w28 = 0xffff;
        ((PL *)D_8009C330)->w2a = 0xffff;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->h->p.whole = o->h->p.whole;
        o->y.p.whole += 4;
        if ((*(unsigned char*)&o->wac) >= 2) {
            D_8009D2E8 = (*(unsigned char **)&PLX.p611c);
            D_8009D2E8[4] = 2;
            D_8009D2E8[5] = 2;
            D_8009D2E8[6] = 0;
        }
        (*(unsigned char*)&o->wac) = 0;
        GVARS.i34 = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xe3) = 0;
        D_8009C330->active = 0;
        setanim(o, 7);
        SfxPlay3(4, 0x7f);
        o->state++;
    case 1:
        AnimAdvance(o);
        if ((*(int*)&GAME.selectedArea) == 0x30000) break;
        if (D_1f8001fc & D_1f8003c6) {
            (*(unsigned char*)((char*)&D_8009C330->w08 + 1)) = 0;
            setanim(o, 9);
            o->b04 = 1;
            o->step = 0xb;
            o->state = 0;
        }
        if (D_1f8001fc & 0x10) {
            (*(unsigned char*)((char*)&D_8009C330->w08 + 1)) = 0;
            setanim(o, 9);
            o->b04 = 1;
            o->step = 0xb;
            o->state = 0;
        }
        if (D_1f8001fc & 0x40) {
            (*(unsigned char*)((char*)&D_8009C330->w08 + 1)) = 0;
            o->y.p.whole += 9;
            o->h->p.whole += (o->animFrame & 1) ? -4 : 4;
            probeCollisionAtDepthC(o, o->h->p.whole, o->y.p.whole);
            o->step = 0xd;
            o->state = 0;
        }
        break;
    }
    FUN_800ef1ac(o);
}
