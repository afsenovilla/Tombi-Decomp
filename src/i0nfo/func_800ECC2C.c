// FUNC 800ecc2c 68 I0NFO
// MATCHING 800ecc2c 68
#include "TOBJ.H"
extern int AnimAdvance(TObj *o);
extern void func_800EC88C(TObj *o, int x, int y, int z);

void func_800ECC2C(TObj *o)
{
    AnimAdvance(o);
    func_800EC88C(o, o->h->p.whole, o->y.p.whole, o->animFrame * 2);
}
