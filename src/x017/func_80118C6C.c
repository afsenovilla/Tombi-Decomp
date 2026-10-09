// FUNC 80118c6c 564 X017
// MATCHING 80118c6c 564
#include "TOBJ.H"
typedef struct {
    short f0, f2, f4, f6, f8, fa, fc, fe, f10, f12, f14, f16, f18, f1a;
} E28;
extern E28 *D_80119A58;
extern unsigned char D_8009CE3A[];
extern TObj *D_8009F2D8[];
extern TObj *FUN_800183b8(void);
extern TObj *FUN_800184d8(void);

void func_80118C6C(TObj *o)
{
    E28 *base;
    E28 *p;
    short i;
    TObj *n;

    base = D_80119A58;
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
            D_8009F2D8[i] = n;
            n->d90 = (int)o;
            if (D_8009CE3A[0]) {
                switch (n->subtype) {
                case 0:
                    n->a.p.whole = 100;
                    n->animFrame = 0;
                    break;
                case 1:
                    n->a.p.whole = 0xa0;
                    n->animFrame = 0;
                    break;
                }
            }
        }
    }
}
