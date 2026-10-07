// FUNC 800ee7e0 172 X000
#include "TOBJ.H"
extern void func_8001fec0(TObj *o);
extern void FUN_8011133c(TObj *o);
extern void FUN_801113bc(TObj *o);
extern void func_8001fd94(TObj *o);
extern short DAT_8009c944;
extern short DAT_8009c946;

void FUN_800ee7e0(TObj *o)
{
    func_8001fec0(o);
    o->h->raw = o->h->raw + (DAT_8009c944 << 7);
    o->y.raw = o->y.raw + (DAT_8009c946 << 7);
    FUN_8011133c(o);
    o->h->raw = o->h->raw + (o->velX << 8);
    o->timer = o->timer - 1;
    if (o->timer <= 0) {
        o->timer = 0;
        FUN_801113bc(o);
        func_8001fd94(o);
    }
}
