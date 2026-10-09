// FUNC 80129c98 608 X009
// MATCHING 80129c98 608
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
extern E D_8012B2E8[];
extern TObj *D_8009F2D8[];
extern unsigned char D_8009CE54;
extern unsigned char D_8009CDCC[];
extern TObj *FUN_800184d8(void);
extern TObj *FUN_800183b8(void);

void func_80129C98(TObj *o)
{
    E *e;
    short i;
    TObj *p;

    for (i = 0, e = D_8012B2E8; e->flags != 0xff; e = &D_8012B2E8[++i]) {
        if (*(volatile unsigned short *)&e->flags & 0x80) {
            TObj *m = FUN_800184d8();
            if (m) {
                m->active = 1;
                m->type = 0x1a;
                m->b0a = 0x10;
                m->animFrame = e->frame;
                m->subtype = e->subtype;
                m->b0c = e->b0c;
                m->a.p.whole = e->x;
                m->y.p.whole = e->y;
                m->b.p.whole = e->z;
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
                D_8009F2D8[i] = p;
                if (*(unsigned short *)&p->wba == 0 && D_8009CE54 == 0xff) p->b04 = 2;
                if (*(unsigned short *)&p->wba == 1) {
                    if (D_8009CDCC[0] == 0xff) p->b04 = 3;
                    if (D_8009CDCC[-2] != 0xff) p->b04 = 3;
                }
            }
        }
    }
}
