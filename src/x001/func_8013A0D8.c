// FUNC 8013a0d8 284 X001
// MATCHING 8013a0d8 284
#include "TOBJ.H"

extern unsigned char D_8009CFFA, D_8009D0B2, D_8009CEAB;
extern TObj *FUN_800183b8(void);

void func_8013A0D8(TObj *o)
{
    TObj *n, *m;
    unsigned char k;

    if (D_8009CFFA == 0 && D_8009D0B2 == 0 && (k = D_8009CEAB) != 0 && k != 0xff) {
        if (k < 3) {
            n = FUN_800183b8();
            if (n) {
                n->active = 1;
                n->type = 0x16;
                n->b0a = 2;
                n->a.p.whole = 0xacd;
                n->animFrame = 0;
                n->subtype = 0;
                n->b0c = 0;
                n->y.p.whole = -0x10a;
                n->b.p.whole = 0;
                *(TObj **)&o->category = n;
            }
            m = FUN_800183b8();
            if (m) {
                m->active = 1;
                m->type = 0x17;
                m->b0a = 2;
                m->a.p.whole = 0x9a8;
                m->animFrame = 0;
                m->subtype = 0;
                m->b0c = 0;
                m->y.p.whole = -0x200;
                m->b.p.whole = 0;
                *(TObj **)&o->timer = m;
                m->d90 = (int)n;
            }
            o->b04 = 1;
        } else {
            o->b04 = 3;
        }
    } else {
        o->b04 = 3;
    }
}
