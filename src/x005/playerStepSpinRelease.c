// FUNC 800f67bc 1144 X005
// MATCHING 800f67bc 1144
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
extern TObj *D_8009C330;
extern TObj *D_8009F0EC;
extern short D_801153C0[];
extern void ObjSetAnimFromTable(TObj *);
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern void SfxPlay2(int, int);
extern void func_8010F254(TObj *);
extern void applyObjectAltSpeedXY(TObj *);
extern int ObjCheckHeadCollision(TObj *);
extern TObj *func_8004BBC0(TObj *, int);
extern void playerEnterGrabStep(TObj *, int);
extern void playerTryAttack(TObj *, int);

static __inline__ void setanim(TObj *o, unsigned short anim, int j)
{
    TObj *p = D_8009C330;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        ObjSetAnimFromTable(o);
        AnimJump(o, j);
        D_8009C330->animFrame = D_8009C330->animTimer;
    }
}

static __inline__ void land(TObj *o)
{
    int v, w;
    setanim(o, 4, 1);
    (*(unsigned char*)&o->wac) = 1;
    o->b9c = 2;
    o->timer = 10;
    (*(unsigned char*)&D_8009C330->w08) = 1;
    v = 0x10;
    o->velY = 0;
    o->velV = 0;
    o->d84 = 0;
    if (o->animFrame & 1) v = 0xf0;
    w = 0xe8;
    o->d88 = v;
    if (o->animFrame & 1) w = 0x18;
    o->d8c = w;
    o->step = 2;
    o->state = 3;
}

void playerStepSpinRelease(TObj *o)
{
    int t;
    TObj *p;
    short k;
    short v;
    switch (o->state) {
    case 0:
        o->b9e = 0;
        o->b69 = 0;
        o->b9c = 1;
        (*(unsigned char*)&o->wac) = 0;
        p = D_8009C330;
        t = 1;
        o->d8c = 0;
        o->wb0 = 0;
        if (o->wb2 > 8) t = 2;
        p->step = t;
        k = D_8009C330->b0b * 7;
        if (o->wb2 < 4) {
            if (o->animFrame & 1) {
                o->velX = -D_801153C0[k + 4] / 3;
            } else {
                o->velX = D_801153C0[k + 4] / 3;
            }
            o->velY = D_801153C0[k + 10] >> 1;
        } else {
            v = D_801153C0[k + o->wb2];
            if (o->animFrame & 1) v = -v;
            o->velX = v;
            o->velY = D_801153C0[k + 10];
        }
        D_8009C330->timer = 0xe;
        o->wb2 = 0;
        o->wb6 = 0;
        setanim(o, 8, 0);
        SfxPlay2(2, 4);
        o->state++;
    case 1:
        o->h->raw += GVARS.s44 << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        func_8010F254(o);
        applyObjectAltSpeedXY(o);
        AnimAdvance(o);
        if (ObjCheckHeadCollision(o)) {
            land(o);
        }
        if (o->velY > 0) {
            land(o);
        }
        break;
    }
    D_8009F0EC = func_8004BBC0(o, 0);
    if (o->b9e != 0) {
        (*(unsigned char*)&D_8009C330->w08) = 0;
        D_8009C330->timer = 0;
        (*(unsigned char*)&o->wac) = 0;
        o->b9c = 0;
        o->velX = 0;
        o->velY = 0;
        o->wb2 = 0;
        playerEnterGrabStep(o, D_8009F0EC == (TObj *)1);
    }
    if ((*(unsigned char*)&o->wac) != 2 && o->step == 9 && o->b9e == 0) {
        playerTryAttack(o, 1);
    }
}
