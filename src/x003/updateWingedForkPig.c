// FUNC 8012f524 1148 X003
// MATCHING 8012f524 1148
// VERSION SLPS_01144 x03 8012E744 1148
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x003 8012F524 (x003/func_8012F524.c): 2550 -> 2575.
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"

typedef struct { TObj o; char pc0[0x12]; unsigned short wd2; } TX;
#define X(o) ((TX *)(o))
typedef struct { short f0, f1, f2, f3, f4, f5; } XS;
extern int ObjCullRegister(TObj *);
extern int Rand(void);
extern void despawnObjectOffscreen(TObj *);
extern void func_8012D740(TObj *);
extern void func_8012D914(TObj *);
extern void func_8012DA68(TObj *);
extern void func_8012DCAC(TObj *);
extern void func_8012DDD4(TObj *);
extern void func_8012E378(TObj *);
extern void func_8012E6B0(TObj *);
extern void func_8012F234(TObj *);
extern void func_8012EB2C(TObj *);
extern void func_8012EF3C(TObj *);
extern void func_8012F3B0(TObj *);

void func_8012F524(TObj *o)
{
    XS *x = (XS *)((char *)o + 0xb4);

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            o->active = 2;
            o->velH = 0;
            o->velV = 0;
            o->ba6 = 0;
            o->velX = 0;
            o->ba7 = 0;
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 8;
            o->box3 = 0x10;
            o->d3c = ((int*)&SPAD.i2dc)[0];
            o->w1e = 1;
            o->b0d = 0;
            o->b69 = 0;
            o->b68 = 0;
            *(signed char *)&o->b0f = -9;
            o->d8c = 0;
            o->anim = 0;
            o->step++;
            x->f0 = 0;
            x->f3 = o->a.p.whole;
            x->f4 = o->y.p.whole;
            x->f5 = o->b.p.whole;
            x->f1 = 0x5f3;
            x->f2 = 0xa0f;
            break;
        case 1:
            if (GVARS.b42 == 0) {
                func_8012D740(o);
            }
            break;
        }
        break;
    case 1:
        if (GVARS.b42) {
            ObjCullRegister(o);
            break;
        }
        ObjCullRegister(o);
        if (o->b9e == 0) {
            switch (o->subtype) {
            case 0:
                switch (o->step) {
                case 0:
                    func_8012D914(o);
                    break;
                case 1:
                    func_8012DA68(o);
                    break;
                }
                break;
            case 1:
                switch (o->step) {
                case 0:
                    func_8012DCAC(o);
                    break;
                case 1:
                    func_8012DDD4(o);
                    break;
                case 2:
                    func_8012E378(o);
                    break;
                case 3:
                    func_8012E6B0(o);
                    break;
                }
                break;
            }
            if (((SPAD.s1f8 + SPAD.i198) & 0xf) == 0) {
                despawnObjectOffscreen(o);
            }
            o->b9d = 0;
            break;
        } else if (o->b9f == 0) {
            X(o)->wd2 = o->h->p.whole;
            o->b9f = 0xf;
        } else {
            { int r = Rand() & 3; int w = X(o)->wd2 - 2; o->h->p.whole = w + r; }
            if (--o->b9f == 0) {
                o->b9e = 0;
                o->h->p.whole = X(o)->wd2;
            }
        }
        o->b9d = 0;
        break;
    case 2:
        if (GVARS.b42) {
            ObjCullRegister(o);
            break;
        }
        switch (o->subtype) {
        case 0:
            switch (o->step) {
            case 1:
                func_8012F234(o);
                ObjCullRegister(o);
                break;
            case 0:
            case 2:
                func_8012F3B0(o);
                if (!ObjCullRegister(o)) {
                    o->b04 = 3;
                }
                break;
            }
            break;
        case 1:
            switch (o->step) {
            case 0:
                func_8012EB2C(o);
                ObjCullRegister(o);
                break;
            case 1:
                func_8012EF3C(o);
                ObjCullRegister(o);
                break;
            case 2:
                func_8012F3B0(o);
                if (!ObjCullRegister(o)) {
                    o->b04 = 3;
                }
                break;
            }
            break;
        }
        break;
    case 3:
        o->timer = 600;
        o->b0b = 0;
        *(signed char *)&o->b0f = -9;
        o->d8c = 0;
        o->b04++;
        break;
    case 4:
        if (--o->timer == -1) {
            x->f0 = 0;
            o->velH = 0;
            o->velV = 0;
            o->velX = 0;
            o->ba7 = 0;
            o->a.p.whole = x->f3;
            o->y.p.whole = x->f4;
            o->b.p.whole = x->f5;
            o->b04 = 0;
            o->step = 1;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
#else
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"

typedef struct { TObj o; char pc0[0x12]; unsigned short wd2; } TX;
#define X(o) ((TX *)(o))
typedef struct { short f0, f1, f2, f3, f4, f5; } XS;
extern int ObjCullRegister(TObj *);
extern int Rand(void);
extern void despawnObjectOffscreen(TObj *);
extern void wingedForkPigWaitAppear(TObj *);
extern void wingedForkPigRise(TObj *);
extern void wingedForkPigFlyAcross(TObj *);
extern void wingedForkPigRiseB(TObj *);
extern void wingedForkPigWander(TObj *);
extern void wingedForkPigChase(TObj *);
extern void wingedForkPigThrow(TObj *);
extern void wingedForkPigGrabbed(TObj *);
extern void wingedForkPigHit(TObj *);
extern void wingedForkPigGrabbedB(TObj *);
extern void wingedForkPigDefeated(TObj *);

void updateWingedForkPig(TObj *o)
{
    XS *x = (XS *)((char *)o + 0xb4);

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            o->active = 2;
            o->velH = 0;
            o->velV = 0;
            o->ba6 = 0;
            o->velX = 0;
            o->ba7 = 0;
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 8;
            o->box3 = 0x10;
            o->d3c = ((int*)&SPAD.i2dc)[0];
            o->w1e = 1;
            o->b0d = 0;
            o->b69 = 0;
            o->b68 = 0;
            *(signed char *)&o->b0f = -9;
            o->d8c = 0;
            o->anim = 0;
            o->step++;
            x->f0 = 0;
            x->f3 = o->a.p.whole;
            x->f4 = o->y.p.whole;
            x->f5 = o->b.p.whole;
            x->f1 = 0x5f3;
            x->f2 = 0x9f6;
            break;
        case 1:
            if (GVARS.b42 == 0) {
                wingedForkPigWaitAppear(o);
            }
            break;
        }
        break;
    case 1:
        if (GVARS.b42) {
            ObjCullRegister(o);
            break;
        }
        ObjCullRegister(o);
        if (o->b9e == 0) {
            switch (o->subtype) {
            case 0:
                switch (o->step) {
                case 0:
                    wingedForkPigRise(o);
                    break;
                case 1:
                    wingedForkPigFlyAcross(o);
                    break;
                }
                break;
            case 1:
                switch (o->step) {
                case 0:
                    wingedForkPigRiseB(o);
                    break;
                case 1:
                    wingedForkPigWander(o);
                    break;
                case 2:
                    wingedForkPigChase(o);
                    break;
                case 3:
                    wingedForkPigThrow(o);
                    break;
                }
                break;
            }
            if (((SPAD.s1f8 + SPAD.i198) & 0xf) == 0) {
                despawnObjectOffscreen(o);
            }
            o->b9d = 0;
            break;
        } else if (o->b9f == 0) {
            X(o)->wd2 = o->h->p.whole;
            o->b9f = 0xf;
        } else {
            { int r = Rand() & 3; int w = X(o)->wd2 - 2; o->h->p.whole = w + r; }
            if (--o->b9f == 0) {
                o->b9e = 0;
                o->h->p.whole = X(o)->wd2;
            }
        }
        o->b9d = 0;
        break;
    case 2:
        if (GVARS.b42) {
            ObjCullRegister(o);
            break;
        }
        switch (o->subtype) {
        case 0:
            switch (o->step) {
            case 1:
                wingedForkPigGrabbed(o);
                ObjCullRegister(o);
                break;
            case 0:
            case 2:
                wingedForkPigDefeated(o);
                if (!ObjCullRegister(o)) {
                    o->b04 = 3;
                }
                break;
            }
            break;
        case 1:
            switch (o->step) {
            case 0:
                wingedForkPigHit(o);
                ObjCullRegister(o);
                break;
            case 1:
                wingedForkPigGrabbedB(o);
                ObjCullRegister(o);
                break;
            case 2:
                wingedForkPigDefeated(o);
                if (!ObjCullRegister(o)) {
                    o->b04 = 3;
                }
                break;
            }
            break;
        }
        break;
    case 3:
        o->timer = 600;
        o->b0b = 0;
        *(signed char *)&o->b0f = -9;
        o->d8c = 0;
        o->b04++;
        break;
    case 4:
        if (--o->timer == -1) {
            x->f0 = 0;
            o->velH = 0;
            o->velV = 0;
            o->velX = 0;
            o->ba7 = 0;
            o->a.p.whole = x->f3;
            o->y.p.whole = x->f4;
            o->b.p.whole = x->f5;
            o->b04 = 0;
            o->step = 1;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
#endif
