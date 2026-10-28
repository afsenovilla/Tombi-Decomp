// FUNC 8011a73c 320 X006
// MATCHING 8011a73c 320
#include "gvars.h"
#include "TOBJ.H"
typedef struct { char p[0x1c]; short w1c; } X;
typedef struct {
    TObj t;
} S;
extern short NEG_SIN_TABLE[];
extern int motocrossBikeBobInit(TObj *);
extern int motocrossBikeModelStepA(TObj *);

void motocrossBikeBob(TObj *o)
{
    TObj *p = (TObj *)o->d90;
    X *x = (X *)((char *)p + 0xb4);

    switch (o->step) {
    case 0:
        o->velY = 0;
        o->velV = 0;
        o->step++;
        *(int *)&o->wb4 = o->d34;
        break;
    case 1:
        if (p->b9c) {
            o->step++;
            break;
        }
        if (x->w1c == 0) {
            o->step = 4;
        }
        break;
    case 2:
        if (p->b9c == 0) {
            motocrossBikeBobInit(o);
            o->step++;
        }
        break;
    case 3:
        if (motocrossBikeBobInit(o) == 1) {
            o->step = 1;
        }
        break;
    case 4:
        motocrossBikeModelStepA(o);
        break;
    }
    if (GVARS.b42 == 0) {
        o->velY = (o->velY + 0x20) & 0xff;
        o->velV = NEG_SIN_TABLE[o->velY];
        o->d34 = (unsigned short)o->wb4 + (o->velV << 4);
    }
}
