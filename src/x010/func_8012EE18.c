// FUNC 8012ee18 180 X010
// MATCHING 8012ee18 180
#include "TOBJ.H"

typedef struct { unsigned short f, sub, c, x, y, z; } SP;
extern SP D_8012F598;
extern TObj *FUN_800183b8(void);

void func_8012EE18(TObj *o)
{
    TObj *n = FUN_800183b8();

    if (n != 0) {
        n->active = 3;
        n->type = 0x32;
        n->animFrame = D_8012F598.f;
        n->subtype = D_8012F598.sub;
        n->b0c = D_8012F598.c;
        n->a.p.whole = D_8012F598.x;
        n->y.p.whole = D_8012F598.y;
        n->b.p.whole = D_8012F598.z;
    }
    o->b04++;
    o->step = 0;
    o->state = 0;
}
