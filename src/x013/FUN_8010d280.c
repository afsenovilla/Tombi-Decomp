// FUNC 8010d280 272 X013
// MATCHING 8010d280 272
#include "tobj.h"
extern short g944, g946;
extern short f1(int, int), f2(int, int);
extern void f3(TObj *), f4(TObj *, int, int), f5(TObj *, int, int, int);
void FUN_8010d280(TObj *o)
{
    if (o->wb2 > 0x400) o->wb2 = 0x400;
    if (o->wb2 < 0) o->wb2 = 0;
    o->h->raw += g944 * 0x100;
    o->y.raw += g946 * 0x100;
    o->velH = f1(o->wb6, o->wb2);
    o->velV = f2(o->wb6, o->wb2);
    o->h->raw += o->velH * 0x100;
    o->y.raw += o->velV * 0x100;
    f3(o);
    f4(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
    f5(o, o->h->p.whole, o->y.p.whole, (short)o->animFrame);
}
