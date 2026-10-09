// FUNC 801343d4 584 X003
// MATCHING 801343d4 584
#include "TOBJ.H"
typedef struct {
    short f0, f2, f4, f6, f8, fa, fc, fe, f10, f12, f14, f16, f18, f1a;
} E28;
extern E28 *D_8013613C;
extern unsigned char D_8009CE5D, D_8009CDC5;
extern unsigned short D_8009C962[];
extern TObj *FUN_800183b8(void);
extern TObj *FUN_800184d8(void);

void func_801343D4(TObj *o)
{
    E28 *base;
    E28 *p;
    short i;
    TObj *n;
    short k;

    base = D_8013613C;
    i = 0;
    p = base;

    for (; p->f0 != 0xff; p = &base[++i]) {
        k = p->f1a == 0x28;
        switch (k) {
        case 0x28:
            if (D_8009CE5D == 0xff) continue;
            break;
        case 0x29:
            if (D_8009CDC5 == 0xff) continue;
            break;
        }
        if (*(volatile unsigned short *)&p->f0 & 0x80) {
            TObj *m;
            m = FUN_800184d8();
            if (m == 0) continue;
            m->active = 1;
            m->type = 0x32;
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
            if (D_8009C962[0] == 1)
                n->subtype = p->f18 + 1;
            else
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
