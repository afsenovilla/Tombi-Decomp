// FUNC 80124848 212 X014
// MATCHING 80124848 212
#include "TOBJ.H"

extern TObj *FUN_800183b8(void);

int func_80124848(TObj *o, unsigned char sub, unsigned char k)
{
    TObj *n = FUN_800183b8();
    int x;

    if (n != 0) {
        n->active = 1;
        n->type = 0x50;
        n->subtype = sub;
        n->b0c = 0;
        n->wb4 = k;
        if (o->animFrame & 1) {
            x = o->a.p.whole - 0x20;
        } else {
            x = o->a.p.whole + 0x20;
        }
        n->a.raw = x << 16;
        n->y.raw = (o->y.p.whole - 4) << 16;
        n->b.raw = o->b.p.whole << 16;
        n->animFrame = o->animFrame & 1;
    }
}
