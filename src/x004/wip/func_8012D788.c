// FUNC 8012d788 556 X004
/* score ~13 (word diffs; ncheck --score crashes on the x004/wip dir when sizes match): only the init of g (&D_8009CE41) differs, the game sets it in the loop preheader (li 1; la g; li 2) while ours sets it before the first loop test. Accessing D_8009CE41[0]/[0x19c] directly gives the right preheader but adds a 16 B frame slot (loop-test temp spilled as ST_REGS) and moves the flags test; a do/while rewrite fixes the init but changes delay-slot fills. Same problem in x017/wip/func_8011890C.c (sibling). do { } while (0) around the first g test flips s4/s5 (debt). */
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
extern E *D_801313C0;
extern TObj *FUN_800184d8(void);
extern TObj *FUN_800183b8(void);
extern unsigned char D_8009CE41[];

void func_8012D788(TObj *o)
{
    E *tbl = D_801313C0;
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
            }
            if (*(unsigned short *)&p->wba == 1) {
                    do {
                        if (g[0] != 1)
                            p->b04 = 2;
                    } while (0);
                    if (g[0x19c] != 0)
                        p->b04 = 2;
                }
        }
    }
}
