// FUNC 80125040 876 X010
// MATCHING 80125040 876
#include "spad.h"
#include "gv2.h"
#include "gvars.h"
/* Box stores as inline setbox(o, 8, 0x10, 8, 0x10); ((int*)&SPAD.i2dc) as a [0] array keeps its load after the struct stores
   (and lets case 0 share the anim tail with case 4). */
#include "TOBJ.H"
#include "raw7.h"

extern void **D_8012F3A8[];
extern void freeObjectLayer2(TObj *);
extern int ObjCullRegister(TObj *);
extern int isObjectInView(TObj *);
extern void AnimLoadDuration(TObj *);
extern void flyingBugWander(TObj *);
extern void flyingBugChase(TObj *);
extern void flyingBugDefeated(TObj *);

#define TBL(o) (*(void ***)((char *)(o) + 0xa8))

static __inline__ void setbox(TObj *o, short a, short b, short c, short d)
{
    o->box0 = a;
    o->box1 = b;
    o->box2 = c;
    o->box3 = d;
}

void updateFlyingBug(TObj *o)
{
    short *sv = &o->wb4;
    void **t;

    switch (o->b04) {
    case 0:
        if (GV2.bc3 & 0x40) {
            freeObjectLayer2(o);
            break;
        }
        o->b0c = o->subtype;
        sv[0] = o->h->p.whole;
        sv[1] = o->y.p.whole;
        sv[2] = o->d->p.whole;
        o->d38 = 0;
        o->d8c = 0;
        t = D_8012F3A8[o->subtype];
        o->b04++;
        setbox(o, 8, 0x10, 8, 0x10);
        *(signed char *)&o->b0f = -9;
        o->w1e = 5;
        TBL(o) = t;
        o->b0d = 0;
        o->d3c = ((int*)&SPAD.i2dc)[0];
        o->wac = 0;
        o->anim = TBL(o)[0];
        AnimLoadDuration(o);
        break;
    case 1:
        if (GVARS.b42) {
            ObjCullRegister(o);
            break;
        }
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            flyingBugWander(o);
            break;
        case 1:
            flyingBugChase(o);
            break;
        }
        if ((unsigned)((o->d38 - 0x40) & 0xff) < 0x80) {
            o->animFrame = 1;
            o->d8c = (o->d38 + 0x80) & 0xff;
        } else {
            o->animFrame = 0;
            o->d8c = o->d38;
        }
        o->b9d = 0;
        o->b69 = 0;
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 1:
            switch (o->state) {
            case 0:
                *(signed char *)&o->b0f = -7;
                o->wac = 3;
                o->b9c = 0;
                o->b0c = o->subtype;
                o->state++;
                o->anim = TBL(o)[3];
                AnimLoadDuration(o);
                break;
            case 1:
                break;
            case 2:
                o->state++;
                break;
            }
            break;
        case 0:
        case 2:
            flyingBugDefeated(o);
            if (!o->visible) o->b04 = 3;
            break;
        }
        break;
    case 3:
        *(signed char *)&o->b0f = -9;
        o->b0b = 0;
        o->h->p.whole = sv[0];
        o->y.p.whole = sv[1];
        o->d->p.whole = sv[2];
        o->b04++;
        break;
    case 4:
        if (((SPAD.s1f8 + SPAD.i198) & 0xf) == 0 && !isObjectInView(o)) {
            o->active = 1;
            o->b9d = 0;
            o->b69 = 0;
            o->d38 = 0;
            o->d30 = 0;
            o->velX = 0;
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            o->wac = 0;
            o->anim = TBL(o)[0];
            AnimLoadDuration(o);
        }
        break;
    }
}
