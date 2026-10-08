// FUNC 8011724c 972 X005
// MATCHING 8011724c 972
#include "TOBJ.H"
typedef struct {
    short f0, f2, f4, f6, f8, fa, fc, fe, f10, f12, f14, f16, f18, f1a;
} E28;
extern E28 *D_80117AC8[];
extern unsigned char D_8009D2C3[];
extern unsigned char D_8009CEED, D_8009D0DA, D_8009CDDE, D_8009CE0C, D_8009D008;
extern TObj *FUN_800183b8(void);
extern TObj *FUN_800184d8(void);

void func_8011724C(TObj *o)
{
    E28 *base;
    E28 *p;
    short i;
    TObj *n;

    base = D_80117AC8[D_8009CEED];
    i = 0;
    p = base;

    for (; p->f0 != 0xff; p = &base[++i]) {
        if (p->fe == 3 && D_8009D0DA == 0) continue;
        if (p->f0 & 0x80) {
            TObj *m = FUN_800184d8();
            if (m == 0) continue;
            m->active = 1;
            m->type = 0x1a;
            m->b0a = 0x10;
            m->animFrame = p->f2;
            m->subtype = p->f18;
            m->b0c = p->f1a;
            m->a.p.whole = p->f12;
            m->y.p.whole = p->f14;
            m->b.p.whole = p->f16;
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
            if (D_8009D2C3[0] & 0x10) {
                n->subtype += 2;
                n->y.p.whole -= 0xc;
            }
        }
    }
    n = FUN_800183b8();
    if (n != 0) {
        n->active = 3;
        n->type = 0x32;
        n->animFrame = 1;
        n->a.p.whole = 0x100;
        n->y.p.whole = -0x30;
        n->b0a = 0;
        n->subtype = 0;
        n->b0c = 0;
        n->b.p.whole = 0;
        n->wb4 = 0;
        n->wb8 = 0;
        n->wba = 5;
        n->w74 = 0;
        n->w76 = 0;
        n->w78 = 0;
        n->w7a = 0;
        n->d90 = (int)o;
        if (D_8009D2C3[0] & 0x10) {
            n->subtype += 2;
            n->y.p.whole -= 0xc;
        }
        if (D_8009CDDE == 0) n->b04 = 2;
    }
    n = FUN_800183b8();
    if (n != 0) {
        n->active = 3;
        n->type = 0x32;
        n->animFrame = 1;
        n->a.p.whole = 0xf2;
        n->y.p.whole = -0x90;
        n->b.p.whole = 0x5a;
        n->b0a = 0;
        n->subtype = 0;
        n->b0c = 0;
        n->wb4 = 0;
        n->wb8 = 0;
        n->wba = 6;
        n->w74 = 0;
        n->w76 = 0;
        n->w78 = 0;
        n->w7a = 0;
        n->b68 = 0;
        n->d90 = (int)o;
        if (D_8009D2C3[0] & 0x10) {
            n->subtype += 2;
            n->y.p.whole -= 0xc;
        }
        if (D_8009CE0C == 0) n->b04 = 2;
        if (D_8009D008 != 0) n->b04 = 2;
    }
}
