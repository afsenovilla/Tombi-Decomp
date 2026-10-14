// FUNC 80134118 548 X003
// MATCHING 80134118 548
#include "gcd.h"
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
extern E *D_801360E4;
extern TObj *allocObjectLayer4(void);
extern TObj *allocObjectLayer2(void);

void func_80134118(TObj *o)
{
    E *tbl = D_801360E4;
    E *e;
    short i;
    TObj *p;
    int k;

    if (GCD.bcdc8 == 0xff) {
        o->b04 = 3;
        return;
    }
    for (i = 0, e = tbl; e->flags != 0xff; e = &tbl[++i]) {
        k = e->b0c == 0x28; /* compared with 0x28 below: kept as in the game */
        if (k == 0x28 && GCD.bcdc8 == 0xff) continue;
        if (*(volatile unsigned short *)&e->flags & 0x80) {
            p = allocObjectLayer4();
            if (p) {
                p->active = 1;
                p->type = 0x32;
                p->b0a = 0x10;
                p->animFrame = e->frame;
                p->subtype = e->subtype;
                p->b0c = e->b0c;
                p->a.p.whole = e->x;
                p->y.p.whole = e->y;
                p->b.p.whole = e->z;
            }
        } else {
            p = allocObjectLayer2();
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
        }
    }
}
