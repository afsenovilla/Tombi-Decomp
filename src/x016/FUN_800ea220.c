// FUNC 800ea220 100 X016
// MATCHING 800ea220 100
#include "TOBJ.H"
extern int DAT_1f8002d4;
extern void *PTR_8013b110[];
extern void func_8001fe6c(TObj *o);

void FUN_800ea220(TObj *o)
{
    int v;
    v = -10;
    o->w1e = 10;
    o->b0d = 0;
    o->b0a = 0;
    if (o->subtype == 0) v = 1;
    *(signed char *)&o->b0f = v;
    o->d3c = DAT_1f8002d4;
    o->anim = PTR_8013b110[o->subtype];
    func_8001fe6c(o);
}
