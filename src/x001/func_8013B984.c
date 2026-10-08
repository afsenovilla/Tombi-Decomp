// FUNC 8013b984 300 X001
// MATCHING 8013b984 300
#include "TOBJ.H"

extern unsigned char D_8009CDB2;
extern int D_8009C984;
extern TObj *FUN_800183b8(void);
extern short func_80139B2C(TObj *);
extern void func_80139C50(TObj *);
extern void PoolFree_1F800210(TObj *);

void func_8013B984(TObj *o)
{
    TObj *e;

    switch (o->b04) {
    case 0:
        if (D_8009CDB2 >= 2 || (D_8009C984 & 0x100)) {
            o->b04 = 3;
            break;
        }
        e = FUN_800183b8();
        if (e) {
            e->active = 1;
            e->type = 0x16;
            e->b0a = 2;
            e->a.p.whole = 0x105b;
            e->y.p.whole = -0x506;
            e->animFrame = 0;
            e->subtype = 0;
            e->b0c = 0;
            e->b.p.whole = 0x47a;
            *(TObj **)&o->category = e;
        }
        o->b04 = 1;
        break;
    case 1:
        if (func_80139B2C(o)) func_80139C50(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
