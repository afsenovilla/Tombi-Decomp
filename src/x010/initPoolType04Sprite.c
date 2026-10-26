// FUNC 800ea220 100 X010
// MATCHING 800ea220 100
#include "TOBJ.H"
extern int DAT_1f8002d4;
extern void *POOL_TYPE04_ANIMS[];
extern void func_8001fe6c(TObj *o);

void initPoolType04Sprite(TObj *o)
{
    int v;
    v = -10;
    o->w1e = 10;
    o->b0d = 0;
    o->b0a = 0;
    if (o->subtype == 0) v = 1;
    *(signed char *)&o->b0f = v;
    o->d3c = DAT_1f8002d4;
    o->anim = POOL_TYPE04_ANIMS[o->subtype];
    func_8001fe6c(o);
}
