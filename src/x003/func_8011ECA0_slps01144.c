// near-twin of springPadContact (PAL)
// VERSION SLPS_01144 x03 8011ECA0 192
#if defined(VERSION_SLPS_01144)
#define objectBoxOverlapSide func_8004E858
extern short D_1F80019E;
#include "spad.h"
#include "TOBJ.H"
extern short objectBoxOverlapSide(TObj *, TObj *);
extern void startPadVibration(int, int, int, int);

void func_8011ECA0_slps01144(TObj *p, TObj *o)
{
    o->b69 = 0;
    if (p->active == 2)
        return;
    if (p->b9e != 0)
        return;
    if (objectBoxOverlapSide(p, o) != 3)
        return;
    o->b69 = 1;
    p->step = 0x33;
    p->b04 = 1;
    p->state = 1;
    p->timer = 0x14;
    if (o->subtype == 0) {
        p->velX = 0x200;
        p->velY = -0x600;
    } else {
        p->velX = 0x180;
        p->velY = -0x980;
    }
    D_1F80019E = 0;
}
#endif
