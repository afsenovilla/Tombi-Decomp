// FUNC 8011fd20 232 X003
// MATCHING 8011fd20 232
#include "TOBJ.H"
extern unsigned char D_1F8001A4;
extern short D_1F80019E;
extern short func_80043464(TObj *, TObj *);
extern void FUN_80025f40(int, int, int, int);

void func_8011FD20(TObj *p, TObj *o)
{
    o->b69 = 0;
    if (p->active == 2)
        return;
    if (p->b9e != 0)
        return;
    if (func_80043464(p, o) != 3)
        return;
    if (D_1F8001A4 != 0)
        return;
    FUN_80025f40(0, 0, 0xff, 4);
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
