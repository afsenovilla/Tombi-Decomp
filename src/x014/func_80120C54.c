// FUNC 80120c54 332 X014
// MATCHING 80120c54 332
// FLAGS -O2 -G0 -fno-strength-reduce
#include "TOBJ.H"

typedef struct { short a, b; } AB;
extern AB D_801265B0[];
extern TObj *FUN_800183b8(void);

void func_80120C54(TObj *o, int k)
{
    TObj *n;
    AB *p;
    char *b;
    int q;
    int m, one;
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
    i = 0;
    m = k & 3;
    one = 1;
    { AB *t = D_801265B0; b = (char *)&t->b; p = t; }
    for (; i < 3; i++, p++) {
        n = FUN_800183b8();
        if (n != 0) {
            n->active = 2;
            n->type = 0x46;
            n->subtype = i + 1;
            n->animFrame = f;
            q = i * 4;
            n->velH = p->a;
            if (m == one) {
                n->velH = -n->velH;
            }
            n->velV = *(short *)(q + (int)b);
            if ((k & 4) == 0) {
                n->a.p.whole = o->a.p.whole + dx;
                n->b0c = 0;
            } else {
                n->velV -= 0x200;
                n->b0c = one;
                n->a.p.whole = o->a.p.whole;
            }
            n->y.p.whole = o->y.p.whole;
            n->b.p.whole = o->b.p.whole;
        }
    }
}
