// FUNC 8012d3b8 524 X010
// MATCHING 8012d3b8 524
#include "TOBJ.H"
typedef struct {
    short f0, f2, f4, f6, f8, fa, fc, fe, f10, f12, f14, f16, f18, f1a;
} E28;
extern unsigned char D_8009CDAB, D_8009CFCC;
extern E28 *D_8012F52C;
extern TObj *FUN_800184d8(void);
extern TObj *FUN_800183b8(void);

void func_8012D3B8(TObj *o)
{
    E28 *base;
    E28 *p;
    short i;
    TObj *n;

    if (D_8009CDAB == 0) {
        o->b04 = 3;
        return;
    }
    if (D_8009CFCC != 0) {
        o->b04 = 3;
        return;
    }
    base = D_8012F52C;
    i = 0;
    p = base;

    for (; p->f0 != 0xff; p = &base[++i]) {
        if (*(volatile unsigned short *)&p->f0 & 0x80) {
            n = FUN_800184d8();
            if (n == 0) continue;
            n->type = 0x1a;
            n->active = 1;
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
            n->active = 1;
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
            n->b68 = 0;
            n->d90 = (int)o;
        }
    }
}
