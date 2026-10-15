// FUNC 80123f24 308 X001
// MATCHING 80123f24 308
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"

extern int ObjCullRegister(TObj *);
extern void setEventStarted(int, int, int);
extern void FUN_80026c50(int, int, int);
extern void freeObjectLayer5(TObj *);

void updateLookAndSeeEventStart(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (GCD.bcdba == 0) {
            volatile int *g = &SPAD.i334;
            int *q = (int *)*g;
            int v;
            q += 9;
            v = *g + *q;
            o->ba4 = 0;
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 8;
            o->box3 = 0x10;
            o->active = 1;
            o->b69 = 0;
            o->b04++;
            o->da0 = v;
        } else {
            o->b04 = 3;
        }
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        if (GCD.bcdba == 0) {
            setEventStarted(0x16, 0, 0);
            FUN_80026c50(0xb, 1, 1);
        }
        o->b04++;
        break;
    case 3:
        freeObjectLayer5(o);
        break;
    }
}
