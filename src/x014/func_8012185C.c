// FUNC 8012185c 240 X014
// MATCHING 8012185c 240
#include "TOBJ.H"
extern TObj *FUN_800183b8(void);

void func_8012185C(TObj *o, int sub)
{
    int i;
    TObj *e;

    for (i = 0; i < 6; i++) {
        e = FUN_800183b8();
        if (e) {
            e->type = 0x47;
            e->active = 2;
            e->subtype = sub;
            e->b0c = i + 1;
            e->b04 = 2;
            e->b0a = o->b0a;
            e->w1e = o->w1e;
            e->b0d = o->b0d;
            e->d3c = o->d3c;
            e->b0f = o->b0f;
            e->a.raw = o->a.p.whole << 16;
            e->y.raw = o->y.p.whole << 16;
            e->b.raw = o->b.p.whole << 16;
        }
    }
}
