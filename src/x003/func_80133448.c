// FUNC 80133448 524 X003
// MATCHING 80133448 524
/* Loop written as a while with the increment in the test (`++i`): the branch target is the copied exit test (NOTE_INSN_LOOP_VTOP), so reorg predicts the `if (e)` skips taken and steals i+1 into their delay slots. Debt: volatile f0 read (keeps the &0x80 load in the body and the frame at 0x28) and do {} while (0) around the second spawn body (extra loop-weighted ref on o so it gets s2 before list). Twin: X004 8012DCF4. */
#include "TOBJ.H"
typedef struct {
    short f0, f2, f4, f6, f8, fa, fc, fe, f10, f12, f14, f16, f18, f1a;
} SP;

extern unsigned char D_8009CDAB, D_8009CFCD;
extern SP *D_8013606C;
extern TObj *FUN_800184d8(void);
extern TObj *FUN_800183b8(void);

void func_80133448(TObj *o)
{
    SP *p, *list;
    TObj *e;
    short i;

    if (!D_8009CDAB || D_8009CFCD) {
        o->b04 = 3;
        return;
    }
    list = D_8013606C;
    i = -1;
    while ((p = &list[++i])->f0 != 0xff) {
            if (*(volatile short *)&p->f0 & 0x80) {
                e = FUN_800184d8();
                if (e) {
                    e->type = 0x1a;
                    e->active = 1;
                    e->b0a = 0x10;
                    e->animFrame = p->f2;
                    e->subtype = p->f18;
                    e->b0c = p->f1a;
                    e->a.p.whole = p->f12;
                    e->y.p.whole = p->f14;
                    e->b.p.whole = p->f16;
                }
            } else {
                e = FUN_800183b8();
                if (e) {
                    do {
                    e->active = 1;
                    e->type = 0x32;
                    e->b0a = 0;
                    e->animFrame = p->f2;
                    e->subtype = p->f18;
                    e->b0c = p->f1a;
                    e->a.p.whole = p->f12;
                    e->y.p.whole = p->f14;
                    e->b.p.whole = p->f16;
                    e->wb4 = p->f4;
                    e->wb8 = p->f10;
                    e->wba = p->fe;
                    e->w74 = p->f6;
                    e->w76 = p->f8;
                    e->w78 = p->fa;
                    e->w7a = p->fc;
                    e->b68 = 0;
                    e->d90 = (int)o;
                    } while (0);
                }
            }
    }
}
