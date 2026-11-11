// VERSION SLPM_80165 x00 800FAC3C 260
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerClimbFollowObject (src/x003/playerClimbFollowObject.c): the grab probe point (+0xD4/+0xD6,
// PAL +0xE8/+0xEA) is 6 right of the player whatever the facing, the probe only answers yes / no (the grabbed
// object is not replaced), a lost grip moves the player by 4 (PAL 8), and +0x0F is left alone.
#include "TOBJ.H"
typedef struct { TObj b; char pad[0xd4 - 0xc0]; short d4, d6; } OX;
extern TObj *GRABBED_OBJECT;
extern int func_8004E29C(TObj *, TObj *);

void func_800FAC3C(TObj *o)
{
    Fix16 *p;
    ((OX *)o)->d4 = o->h->p.whole + 6;
    ((OX *)o)->d6 = o->y.p.whole - 8;
    if (func_8004E29C(o, GRABBED_OBJECT) == 0) {
        p = o->h;
        {
            int t2, h2;
            t2 = o->animFrame & 1;
            h2 = p->p.whole;
            if (t2) t2 = h2 + 4; else t2 = h2 - 4;
            p->p.whole = t2;
        }
        *(char *)&o->wac = 1;
        o->step = 2;
        o->velX = 0;
        o->velY = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->b9e = 0;
        *(char *)&o->waa = 0;
        o->d8c = 0;
        o->state = 3;
    } else {
        TObj *n = GRABBED_OBJECT;
        o->h->p.whole = n->h->p.whole + o->wb8;
        o->y.p.whole = n->y.p.whole + o->wba + 8;
        if (o->b9e == 11) {
            o->step = 11;
            o->state = 0;
        }
    }
}
#endif
