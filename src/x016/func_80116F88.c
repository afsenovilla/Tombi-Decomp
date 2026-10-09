// FUNC 80116f88 500 X016
// MATCHING 80116f88 500
#include "TOBJ.H"
typedef struct {
    short f0, f2, f4, f6, f8, fa, fc, fe, f10, f12, f14, f16, f18, f1a;
} E28;
extern E28 *D_80118CD4;
extern unsigned char D_8009CDED;
extern TObj *FUN_800183b8(void);
extern TObj *FUN_800184d8(void);

void func_80116F88(TObj *o)
{
    E28 *base;
    E28 *p;
    short i;
    TObj *n;

    if (D_8009CDED != 0xff) {
        o->b04 = 2;
        return;
    }
    base = D_80118CD4;
    i = 0;
    p = base;

    for (; p->f0 != 0xff; p = &base[++i]) {
        if (*(volatile unsigned short *)&p->f0 & 0x80) {
            n = FUN_800184d8();
            if (n == 0) continue;
            n->active = 1;
            n->type = 0x1a;
            n->b0a = 0x10;
            n->animFrame = p->f2;
            n->subtype = p->f18;
            n->b0c = p->f1a;
            n->a.p.whole = p->f12;
            n->y.p.whole = p->f14;
            n->b.p.whole = p->f16;
        } else {
            n = FUN_800183b8();
            if (n == 0) continue;
            n->active = 3;
            n->type = 0x32;
            n->b0a = 0;
            n->animFrame = p->f2;
            n->subtype = p->f18;
            n->b0c = p->f1a;
            n->a.p.whole = p->f12;
            n->y.p.whole = p->f14;
            n->b.p.whole = p->f16;
            n->wb4 = p->f4;
            n->wb8 = p->f10;
            n->wba = p->fe;
            n->w74 = p->f6;
            n->w76 = p->f8;
            n->w78 = p->fa;
            n->w7a = p->fc;
            n->d90 = (int)o;
        }
    }
}
