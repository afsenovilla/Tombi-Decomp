// FUNC 80124410 144 X001
// MATCHING 80124410 144
#include "TOBJ.H"

typedef struct {
    TObj o;
    unsigned char pad[0x24];
    TObj *e4;
} P;

extern short FUN_800435e0(P *, TObj *);
extern void FUN_8001f96c(int, short, short, short);

void func_80124410(P *a, TObj *b)
{
    short r = FUN_800435e0(a, b);

    if (r != -1 && r < 3 && *(unsigned char *)&a->o.wac == 1) {
        b->active = 2;
        b->b04 = 2;
        b->step = 0;
        b->state = 0;
        b->b69 = 0;
        a->e4 = b;
        *(unsigned char *)&a->o.wac = 2;
        FUN_8001f96c(2, a->o.a.p.whole, a->o.y.p.whole, a->o.b.p.whole);
    }
}
