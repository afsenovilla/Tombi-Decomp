// FUNC 80117228 460 X016
// MATCHING 80117228 460
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
extern E *D_80118D10;
extern TObj *FUN_800184d8(void);
extern TObj *FUN_800183b8(void);

void func_80117228(TObj *o)
{
    E *tbl = D_80118D10;
    E *e;
    short i;
    TObj *p;

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
        }
    }
}
