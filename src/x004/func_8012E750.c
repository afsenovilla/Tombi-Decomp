// FUNC 8012e750 196 X004
// MATCHING 8012e750 196
#include "TOBJ.H"
typedef struct { unsigned short f, sub, c; short x, y, z; } Spawn;
extern unsigned char D_8009CE22;
extern Spawn D_8013143C;
extern TObj *FUN_800183b8(void);

void func_8012E750(TObj *o)
{
    TObj *e;

    if (D_8009CE22) {
        e = FUN_800183b8();
        if (e) {
            e->active = 3;
            e->type = 0x32;
            e->animFrame = D_8013143C.f;
            e->subtype = D_8013143C.sub;
            e->b0c = D_8013143C.c;
            e->a.p.whole = D_8013143C.x;
            e->y.p.whole = D_8013143C.y;
            e->b.p.whole = D_8013143C.z;
        }
    }
    o->step = 0;
    o->state = 0;
    o->b04++;
}
