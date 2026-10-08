// FUNC 8002ffb0 132 MAIN0
// MATCHING 8002ffb0 132
#include "TOBJ.H"
extern TObj *ObjAlloc();
extern int D_800A6048[];

void func_8002FFB0(int a, int b)
{
    TObj *o = ObjAlloc();
    if (o) {
        o->active = 1;
        o->type = 0x4e;
        o->subtype = a;
        o->b0c = b;
        o->a.raw = D_800A6048[0];
        o->y.raw = D_800A6048[1];
        o->b.raw = D_800A6048[2];
        o->step = 1;
    }
}
