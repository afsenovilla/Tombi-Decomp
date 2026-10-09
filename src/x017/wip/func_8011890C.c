// FUNC 8011890c 548 X017
/* Real size 548 B: includes the 4-byte csv piece func_8011890C (the prologue). */
/* score ~13 (was 21; do/while(0) around the first g test fixes s4/s5): g init placement differs; s4/s5 swapped between g (&D_8009CE41, game hoists it into the loop preheader) and the hoisted constant 1. Direct D_8009CE41[0]/[0x1a0] accesses give the right regs but add a 16-byte frame slot (a loop temp of e = &tbl[++i] gets class ST_REGS and is spilled) and move the flags test. Tried: g set before/inside the loop, for-init, do/while rewrite, struct global, register keyword. (o19) The extra 16 B with direct D_8009CE41 accesses is a combine leftover `(use (reg 79))` of the loop-test sign extension (lh of e->flags; see cc1 -dl): the HI pseudo of the test is kept live into the body (lhu a0 in the test block), with or without volatile; -fno-strength-reduce/-fno-cse-* do not help; g assigned at its use (44) or in a preheader if/do-while (55) is worse. */
#include "TOBJ.H"
typedef struct {
    short flags;
    short frame;
    short wb4;
    short w74, w76, w78, w7a;
    short wba, wb8;
    short x, y, z;
    short subtype;
    short b0c;
} E;
extern E *D_80119A00;
extern TObj *FUN_800184d8(void);
extern TObj *FUN_800183b8(void);
extern unsigned char D_8009CE41[];

void func_8011890C(TObj *o)
{
    E *tbl = D_80119A00;
    E *e;
    short i;
    TObj *p;
    unsigned char *g = D_8009CE41;

    for (i = 0, e = tbl; e->flags != 0xff; e = &tbl[++i]) {
        if (*(volatile unsigned short *)&e->flags & 0x80) {
            p = FUN_800184d8();
            if (p) {
                p->active = 1;
                p->type = 0x1a;
                p->b0a = 0x10;
                p->animFrame = e->frame;
                p->subtype = e->subtype;
                p->b0c = e->b0c;
                p->a.p.whole = e->x;
                p->y.p.whole = e->y;
                p->b.p.whole = e->z;
            }
        } else {
            p = FUN_800183b8();
            if (p) {
                p->active = 3;
                p->type = 0x32;
                p->b0a = 0;
                p->animFrame = e->frame;
                p->subtype = e->subtype;
                p->b0c = e->b0c;
                p->a.p.whole = e->x;
                p->y.p.whole = e->y;
                p->b.p.whole = e->z;
                p->wb4 = e->wb4;
                p->wb8 = e->wb8;
                p->wba = e->wba;
                p->w74 = e->w74;
                p->w76 = e->w76;
                p->w78 = e->w78;
                p->w7a = e->w7a;
                p->d90 = (int)o;
                if (*(unsigned short *)&p->wba == 1) {
                    do {
                        if (g[0] != 1)
                            p->b04 = 2;
                    } while (0);
                    if (g[0x1a0] != 0)
                        p->b04 = 2;
                }
            }
        }
    }
}
