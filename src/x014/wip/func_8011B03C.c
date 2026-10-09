// FUNC 8011b03c 308 X014
/* score 38: only o's register differs (game a3, ours a1): in the game o conflicts with a1/a2, which hold D_1F800176/D_1F800186 in case 1 and are passed on to FUN_80018da4 (3 args here, the game leaves them in a1/a2 at the call). Global alloc gives o a1 because cse rewrites case 1 to use the a0 arg copy, so o dies before a1/a2 are set. Tried: x/y types and scopes, x/y as reused params, all store orders, register asm("$7") (37).
   o29: dumps show sched1 hoists the call-arg copy a0=o (insn kills o, priority boost) above the x/y loads, then local-alloc
   rewrites the stores to a0; game must have had the copy below the loads. Same root cause as X010 func_8011B1F8 (o in t0).
   Tried K&R/ushort/short/void* prototypes, p=o copies (top/after loads), return after call: all 38. */
#include "TOBJ.H"
extern void *D_8012A01C[];
extern int D_1F8002D4[];
extern unsigned short D_1F800176;
extern unsigned short D_1F800186;
extern void FUN_80018da4(TObj *, int, int);
extern void FUN_80018934(TObj *);

void func_8011B03C(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->w1e = 13;
        o->y.p.whole += 0x60;
        o->b0d = 0;
        o->anim = D_8012A01C[o->subtype];
        o->d3c = D_1F8002D4[0];
        o->b0f = 3;
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->b04++;
        break;
    case 1:
        {
            unsigned short x = D_1F800176, y = D_1F800186;
            o->visible = 1;
            o->b.p.whole = 0;
            o->a.p.whole = (short)(o->d30 - x) >> 2;
            o->y.p.whole = (short)(o->d34 - y) >> 2;
            FUN_80018da4(o, x, y);
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018934(o);
        break;
    }
}
