// VERSION SLPM_80165 x00 800EB94C 156
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only: spawns a type 0 object of subtype 0 / variant 5 at o's position (same +0x0F, +0x1D and +0xA0, +0x0A
// 0x10); returns it, or 0 when no slot is free.
#include "TOBJ.H"
extern TObj *func_80017280(void);

TObj *func_800EB94C(TObj *o)
{
    TObj *n = func_80017280();
    if (n == 0)
        return 0;
    *(int *)((char *)n + 0xa0) = *(int *)((char *)o + 0xa0);
    n->b0a = 0x10;
    n->subtype = 0;
    n->active = 1;
    n->b1d = o->b1d;
    n->type = 0;
    n->b0c = 5;
    n->animFrame = 0;
    n->b0f = o->b0f;
    n->a = o->a;
    n->y = o->y;
    n->b = o->b;
    return n;
}
#endif
