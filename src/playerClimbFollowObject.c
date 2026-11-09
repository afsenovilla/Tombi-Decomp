// FUNC 8010c7b0 292 X000
// MATCHING 8010c7b0 292
// VERSION SLPS_01144 x00 8010C244 332
// VERSION BETA_980203 x00 80110658 292
#if defined(VERSION_SLPS_01144)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct { TObj b; char pad[0xd8 - 0xc0]; short e8, ea; } OX;
extern TObj *GRABBED_OBJECT;
extern TObj *probeClimbGrabObject(TObj *, TObj *);
extern void playerSetFallLean(TObj *);

void func_8010C244(TObj *o)
{
    TObj *n;
    int v;
    Fix16 *p;
    {
        int t1, h1;
        h1 = o->h->p.whole;
        if (o->animFrame & 1) t1 = h1 - 6; else t1 = h1 + 6;
        ((OX *)o)->e8 = t1;
    }
    ((OX *)o)->ea = o->y.p.whole - 8;
    n = probeClimbGrabObject(o, GRABBED_OBJECT);
    if (n == 0) {
        *(signed char *)&o->b0f = -8;
        p = o->h;
        {
            int t2, h2;
            t2 = o->animFrame & 1;
            h2 = p->p.whole;
            if (t2) t2 = h2 + 8; else t2 = h2 - 8;
            p->p.whole = t2;
        }
        v = 0x10;
        o->velX = 0;
        o->velY = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->b9e = 0;
        *(char *)&o->waa = 0;
        *(char *)&o->wac = 1;
        o->d84 = 0;
        if (o->animFrame & 1) v = 0xf0;
        o->timer = 10;
        o->step = 2;
        o->d88 = v;
        o->d8c = 0;
        o->state = 3;
    } else {
        o->h->p.whole = n->h->p.whole + o->wb8;
        GRABBED_OBJECT = n;
        o->y.p.whole = n->y.p.whole + o->wba + 8;
        if (o->b9e == 11) {
            *(signed char *)&o->b0f = -8;
            o->step = 11;
            o->state = 0;
        }
    }
}
#endif
#elif defined(VERSION_BETA_980203)
// Beta version of PAL x000 8010C7B0 (playerClimbFollowObject.c): PAL C of src/playerClimbFollowObject.c, edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
typedef struct { TObj b; char pad[0xdc - 0xc0]; short e8, ea; } OX;
extern TObj *GRABBED_OBJECT;
extern TObj *probeClimbGrabObject(TObj *, TObj *);
extern void playerSetFallLean(TObj *);

void playerClimbFollowObject(TObj *o)
{
    TObj *n;
    int h, t, f;
    Fix16 *p;
    {
        int t1, h1;
        t1 = o->animFrame & 1;
        h1 = o->h->p.whole;
        if (t1) t1 = h1 - 6; else t1 = h1 + 6;
        ((OX *)o)->e8 = t1;
    }
    ((OX *)o)->ea = o->y.p.whole - 8;
    n = probeClimbGrabObject(o, GRABBED_OBJECT);
    if (n == 0) {
        *(signed char *)&o->b0f = -8;
        p = o->h;
        {
            int t2, h2;
            t2 = o->animFrame & 1;
            h2 = p->p.whole;
            if (t2) t2 = h2 + 8; else t2 = h2 - 8;
            p->p.whole = t2;
        }
        o->velX = 0;
        o->velY = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->b9e = 0;
        *(char *)&o->waa = 0;
        *(char *)&o->wac = 1;
        playerSetFallLean(o);
        o->step = 2;
        o->state = 3;
    } else {
        o->h->p.whole = n->h->p.whole + o->wb8;
        GRABBED_OBJECT = n;
        o->y.p.whole = n->y.p.whole + o->wba + 8;
        if (o->b9e == 11) {
            *(signed char *)&o->b0f = -8;
            o->step = 11;
            o->state = 0;
        }
    }
}
#else
#include "TOBJ.H"
typedef struct { TObj b; char pad[0xe8 - 0xc0]; short e8, ea; } OX;
extern TObj *GRABBED_OBJECT;
extern TObj *probeClimbGrabObject(TObj *, TObj *);
extern void playerSetFallLean(TObj *);

void playerClimbFollowObject(TObj *o)
{
    TObj *n;
    int h, t, f;
    Fix16 *p;
    {
        int t1, h1;
        t1 = o->animFrame & 1;
        h1 = o->h->p.whole;
        if (t1) t1 = h1 - 6; else t1 = h1 + 6;
        ((OX *)o)->e8 = t1;
    }
    ((OX *)o)->ea = o->y.p.whole - 8;
    n = probeClimbGrabObject(o, GRABBED_OBJECT);
    if (n == 0) {
        *(signed char *)&o->b0f = -8;
        p = o->h;
        {
            int t2, h2;
            t2 = o->animFrame & 1;
            h2 = p->p.whole;
            if (t2) t2 = h2 + 8; else t2 = h2 - 8;
            p->p.whole = t2;
        }
        o->velX = 0;
        o->velY = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->b9e = 0;
        *(char *)&o->waa = 0;
        *(char *)&o->wac = 1;
        playerSetFallLean(o);
        o->step = 2;
        o->state = 3;
    } else {
        o->h->p.whole = n->h->p.whole + o->wb8;
        GRABBED_OBJECT = n;
        o->y.p.whole = n->y.p.whole + o->wba + 8;
        if (o->b9e == 11) {
            *(signed char *)&o->b0f = -8;
            o->step = 11;
            o->state = 0;
        }
    }
}
#endif
