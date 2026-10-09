// FUNC 801199f0 420 X011
// MATCHING 801199f0 420
#include "TOBJ.H"
typedef struct {
    short f0, f2, f4, f6, f8, fa, fc, fe, f10, f12, f14, f16, f18, f1a;
} E28;
extern E28 *D_80119CFC;
extern unsigned char D_8009CE41[];
extern TObj *FUN_800183b8(void);

void func_801199F0(TObj *o)
{
    E28 *tbl = D_80119CFC;
    E28 *e;
    short i;
    TObj *p;

    for (i = 0, e = tbl; e->f0 != 0xff; e = &tbl[++i]) {
        p = FUN_800183b8();
        if (p) {
            p->active = 3;
            p->type = 0x32;
            p->b0a = 0;
            p->animFrame = e->f2;
            p->subtype = e->f18;
            p->b0c = e->f1a;
            p->a.p.whole = e->f12;
            p->y.p.whole = e->f14;
            p->b.p.whole = e->f16;
            p->wb4 = e->f4;
            p->wb8 = e->f10;
            p->wba = e->fe;
            p->w74 = e->f6;
            p->w76 = e->f8;
            p->w78 = e->fa;
            p->w7a = e->fc;
            p->d90 = (int)o;
            if (i == 3) {
                if (D_8009CE41[0] != 1) p->b04 = 2;
                if (D_8009CE41[0x19e] != 0) p->b04 = 2;
            }
        }
    }
}
