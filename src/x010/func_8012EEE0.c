// FUNC 8012eee0 276 X010
// MATCHING 8012eee0 276
#include "TOBJ.H"
typedef struct { unsigned short af, st, c, x, y, z; } SP;

extern SP D_8012F598;
extern TObj *FUN_800183b8(void);
extern void PoolFree_1F800210(TObj *);

void func_8012EEE0(TObj *o)
{
    TObj *e;

    switch (o->b04) {
    case 0:
        e = FUN_800183b8();
        if (e) {
            e->active = 3;
            e->type = 0x32;
            e->animFrame = D_8012F598.af;
            e->subtype = D_8012F598.st;
            e->b0c = D_8012F598.c;
            e->a.p.whole = D_8012F598.x;
            e->y.p.whole = D_8012F598.y;
            e->b.p.whole = D_8012F598.z;
        }
        o->b04++;
        o->step = 0;
        o->state = 0;
        break;
    case 1:
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
