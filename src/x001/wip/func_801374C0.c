/* score 23: scheduling only: game hoists the D_800A6066 load right after the type store and li s0,2 before the subtype store, then stores subtype, w1e, b0d, animFrame, anim, b0a, d3c in that order. Tried statement permutations (hill climb), local temp for the player animFrame, struct vs scalar extern. */
// FUNC 801374c0 220 X001
#include "TOBJ.H"
extern unsigned short D_800A6066;
extern void *D_8013E568;
extern int D_1F8002D4;
extern TObj *FUN_800183b8(void);
extern void AnimLoadDuration(TObj *);
void func_801374C0(unsigned char sub, short x, short y, short z)
{
    TObj *o = FUN_800183b8();
    if (o != 0) {
        o->active = 1;
        o->type = 0x17;
        o->a.raw = x << 16;
        o->y.raw = y << 16;
        o->b.raw = z << 16;
        o->w1e = 10;
        o->b0d = 0;
        o->anim = D_8013E568;
        o->b0a = 2;
        o->subtype = sub;
        o->animFrame = D_800A6066 & 1;
        o->d3c = D_1F8002D4;
        AnimLoadDuration(o);
        o->b04 = 2;
        o->step = 3;
        o->state = 0;
    }
}
