// FUNC 801301c0 396 X003
// MATCHING 801301c0 396
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void *D_8013A368[];
extern void *D_8013A378[];
extern int showMessageBoxDirect(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void phoenixGossipAskTalk(TObj *o)
{
    TObj *p;
    unsigned char *k;

    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68 == 0) break;
        o->state++;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->d90 = showMessageBoxDirect(2, 0xd, &o->a);
        o->wac = 4;
        o->animFrame = o->b68 & 1;
        setAnim(o, D_8013A378[0]);
        break;
    case 2:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->wac = 0;
        o->anim = D_8013A368[0];
        o->state = 0;
        k = &GCD.bd00a;
        if (*k) {
            GVARS.b3f = 0;
            GVARS.b42 = 0;
            PLAYER.obj.b04 = 1;
            PLAYER.obj.step = 0;
            PLAYER.obj.state = 0;
        } else {
            *k = 1;
        }
        break;
    }
}
