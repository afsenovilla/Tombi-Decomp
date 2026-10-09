// FUNC 80133448 524 X003
/* score 36: logic and frame match. Left: global-alloc puts the hoisted constant 1 in s3 and list in s4 (game: list s3,
   const s4; -dg: list 3 refs/76 insns loses to const 5/142), and the `if (e)` branches fill their delay slot from the
   fallthrough instead of the i+1 at the loop tail. Tried for/do-while/goto/continue loop forms, list vs p tests,
   declaration orders, a `one` variable. Twin: X004 8012DCF4. */
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
    {
        list = D_8013606C;
        p = list;
        if (list->f0 != 0xff) {
        i = 0;
        do {
            if (p->f0 & 0x80) {
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
                }
            }
            i++;
            p = &list[i];
        } while (p->f0 != 0xff);
        }
    }
}
