// FUNC 80120c54 332 X014
/* score 67: loop giv/regalloc differ (game: s2 = &T[i].a stepping, fp = &T[0].b indexed by i*4, s7 = k&3 hoisted); tried 2D array, pointer p/q forms */
#include "TOBJ.H"

typedef struct { short a, b; } AB;
extern AB D_801265B0[];
extern TObj *FUN_800183b8(void);

void func_80120C54(TObj *o, int k)
{
    TObj *n;
    AB *p;
    int i;
    short dx;
    short f;

    if ((k & 3) == 2) {
        dx = -0x24;
        f = 1;
    } else {
        dx = 0x24;
        f = 0;
    }
    p = D_801265B0;
    for (i = 0; i < 3; i++, p++) {
        n = FUN_800183b8();
        if (n != 0) {
            n->active = 2;
            n->type = 0x46;
            n->subtype = i + 1;
            n->animFrame = f;
            n->velH = p->a;
            if ((k & 3) == 1) {
                n->velH = -n->velH;
            }
            n->velV = D_801265B0[i].b;
            if (k & 4) {
                n->velV -= 0x200;
                n->b0c = 1;
                n->a.p.whole = o->a.p.whole;
            } else {
                n->b0c = 0;
                n->a.p.whole = o->a.p.whole + dx;
            }
            n->y.p.whole = o->y.p.whole;
            n->b.p.whole = o->b.p.whole;
        }
    }
}
