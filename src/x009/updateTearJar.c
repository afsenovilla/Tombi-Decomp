// FUNC 8011b6bc 548 X009
// MATCHING 8011b6bc 548
#include "gcd.h"
#include "TOBJ.H"
typedef struct { unsigned short b0, b1, b2, b3, w1e; short k; void **anims; } T16;
extern T16 TEAR_JAR_DEFS[];
extern int SPR_DATA[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void stepTearJar(TObj *);
extern void ObjFreeDup(TObj *);

void updateTearJar(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->box0 = TEAR_JAR_DEFS[o->subtype].b0;
        o->box1 = TEAR_JAR_DEFS[o->subtype].b1;
        o->box2 = TEAR_JAR_DEFS[o->subtype].b2;
        o->box3 = TEAR_JAR_DEFS[o->subtype].b3;
        o->w1e = TEAR_JAR_DEFS[o->subtype].w1e;
        o->d3c = SPR_DATA[TEAR_JAR_DEFS[o->subtype].k];
        o->anim = *TEAR_JAR_DEFS[o->subtype].anims;
        o->d8c = 0;
        o->b0d = 0;
        o->category |= 0x80;
        o->b0a = 2;
        AnimLoadDuration(o);
        o->b04 = 1;
        break;
    case 1:
        ObjCullRegister(o);
        stepTearJar(o);
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            o->step = 1;
            break;
        case 1:
            AnimAdvance(o);
            break;
        case 2:
            o->step = 5;
            o->state = 0;
            break;
        case 3:
            o->b04 = 3;
            break;
        }
        break;
    case 3:
        GCD.bcff8 = 0;
        ObjFreeDup(o);
        break;
    }
}
