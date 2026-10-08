// FUNC 80136a98 296 X000
// MATCHING 80136a98 296
#include "TOBJ.H"
extern TObj *FUN_800183b8(void);
extern unsigned char DAT_8009cda9;
extern unsigned DAT_8009c984[];

void FUN_80136a98(TObj *o)
{
    TObj **p = (TObj **)&o->category;
    TObj *n;
    unsigned f;
    switch (DAT_8009cda9) {
    case 0:
        n = FUN_800183b8();
        if (n != 0) {
            n->active = 1;
            n->type = 0x14;
            n->a.p.whole = 0xa38;
            n->y.p.whole = -0x124;
            n->animFrame = 0;
            n->subtype = 0;
            n->b.p.whole = -0x28;
            *(TObj **)&o->timer = n;
        }
        o->b04 = 1;
        break;
    case 0xff:
        o->b04 = 3;
        break;
    default:
        n = FUN_800183b8();
        if (n != 0) {
            n->active = 1;
            n->type = 0x14;
            n->a.p.whole = 0xa38;
            n->y.p.whole = -0x124;
            n->animFrame = 0;
            n->subtype = 0;
            n->b.p.whole = -0x28;
            f = DAT_8009c984[0];
            if (f & 8) if (f & 0x10) {
                n->a.p.whole = 0xc29;
                n->y.p.whole = -0x2e3;
                n->active = 1;
                n->type = 0x14;
                n->animFrame = 0;
                n->subtype = 1;
                n->b.p.whole = 0x5a;
            }
            p[1] = n;
        }
        o->b04 = 1;
        break;
    }
}
