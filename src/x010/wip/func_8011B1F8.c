/* score 36: everything matches except the register of o: game keeps o in t0 (move t0,a0), here global-alloc gives it a1
   (o dies at the a0 copy in case 1 before the a1..a3 global loads). Tried: block/function-scope temps in case 1, pointer copy,
   register asm (worse), scalar/[0] forms + statement order search for case 0. */
// FUNC 8011b1f8 312 X010
#include "TOBJ.H"
extern void *D_80131C88;
extern int D_1F8002D4[];
extern unsigned short D_1F800176;
extern unsigned short D_1F800186;
extern int D_800A4570;
extern void FUN_80018da4(TObj *);
extern void FUN_80018934(TObj *);

void func_8011B1F8(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b0d = 0;
        o->w1e = 8;
        o->y.p.whole += 0x20;
        o->anim = D_80131C88;
        o->d3c = D_1F8002D4[0];
        o->b0f = 3;
        o->d30 = o->h->p.whole;
        o->b04++;
        o->d34 = o->y.p.whole;
        break;
    case 1: {
        unsigned short cx = D_1F800176, cy = D_1F800186;
        int sc = D_800A4570;
        o->visible = 1;
        o->b.p.whole = 0;
        o->a.p.whole = (short)(o->d30 - cx) >> 1;
        o->y.p.whole = (short)(o->d34 - cy - ((sc >> 8) << 3)) >> 1;
        FUN_80018da4(o);
        break;
    }
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018934(o);
        break;
    }
}
