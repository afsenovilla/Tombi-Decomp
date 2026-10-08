// FUNC 80139a80 172 X001
// MATCHING 80139a80 172
#include "TOBJ.H"

extern unsigned char D_8009CDB2;
extern int D_8009C984;
extern TObj *FUN_800183b8(void);

void func_80139A80(TObj *o)
{
    TObj *n;

    if (D_8009CDB2 < 2) {
        if (D_8009C984 & 0x100) {
            o->b04 = 3;
        } else {
            n = FUN_800183b8();
            if (n) {
                n->active = 1;
                n->type = 0x16;
                n->b0a = 2;
                n->a.p.whole = 0x105b;
                n->y.p.whole = -0x506;
                n->animFrame = 0;
                n->subtype = 0;
                n->b0c = 0;
                n->b.p.whole = 0x47a;
                *(TObj **)&o->category = n;
            }
            o->b04 = 1;
        }
    } else {
        o->b04 = 3;
    }
}
