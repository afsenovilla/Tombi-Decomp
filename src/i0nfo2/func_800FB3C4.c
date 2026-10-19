// FUNC 800fb3c4 916 I0NFO2
// MATCHING 800fb3c4 916
#include "TOBJ.H"
#include "raw7.h"
typedef struct {
    short type, w2, frame, w6, w8, wA, wC, wE, w10, w12, x, y, sub, bc;
} SpawnA;
typedef struct {
    short type, w2, w4, w6, x, y, sub, bc;
} SpawnB;
extern TObj *func_800183B8();
extern TObj *func_80018448();
extern unsigned char D_8009CDF1;
extern unsigned char D_8009CE0A;
extern unsigned char D_8009CF60;
extern unsigned char D_8009CFA1;
extern unsigned char D_8009CFA8;
extern TObj *D_8009F2D8[];
extern SpawnB *D_801001D4;
extern SpawnB *D_801001D8;
extern SpawnA *D_80100364;

void func_800FB3C4(TObj *parent0)
{
    register TObj *parent asm("s1") = parent0;
    SpawnA *tbl;
    SpawnA *p;
    SpawnB *e;
    TObj *o;
    short i;
    TObj **lp;
    unsigned short w;

    if (D_8009CDF1 == 0xFF) {
        D_8009CF60 = 3;
    } else if (D_8009CE0A != 0) {
        D_8009CF60 = 2;
    } else if (D_8009CDF1 != 0) {
        D_8009CF60 = 1;
    } else {
        D_8009CF60 = 0;
    }
    tbl = D_80100364;
    for (i = 0; (p = &tbl[i])->type != 0xFF; i++) {
        lp = D_8009F2D8;
        switch (p->bc) {
        case 8:
            if (D_8009CF60 == 0) break;
            goto spawn;
        case 11:
            if (D_8009CF60 != 0) break;
            goto spawn;
        case 12:
            if (D_8009CF60 != 0) break;
            goto spawn;
        case 15:
            if (D_8009CF60 != 3) break;
        default:
        spawn:
            o = func_800183B8();
            if (o != 0) {
                o->active = 1;
                o->type = 0x30;
                o->b0a = 0xD;
                o->animFrame = p->frame;
                o->subtype = p->sub;
                o->b0c = p->bc;
                o->_pad0e[0] = p->type;
                o->a.p.whole = p->x;
                o->y.p.whole = p->y;
                o->b.p.whole = 0;
                o->wb4 = p->w6;
                o->wb6 = p->w2;
                o->wb8 = p->w12;
                o->wba = p->w10;
                o->w74 = p->x + p->w8;
                o->w76 = p->y + p->wA;
                o->w78 = p->x + p->wC;
                o->w7a = p->y + p->wE;
                lp[i] = o;
                o->d90 = (int)parent;
            }
            break;
        }
    }
    e = D_801001D4;
    if (D_8009CFA8 == 4) {
        e = D_801001D8;
    }
    i = 0;
    for (; e->type != 0xFF; e++) {
        o = func_80018448();
        if (o != 0) {
            o->active = 1;
            o->type = 0x1D;
            o->b0a = 0xD;
            o->animFrame = 0;
            o->subtype = e->sub;
            o->b0c = e->bc;
            o->_pad0e[0] = e->type;
            o->a.p.whole = e->x;
            o->y.p.whole = e->y;
            o->b.p.whole = 0;
            w = e->w6;
            o->wb4 = w;
            if (i == 0xF) {
                o->wb4 = w + D_8009CFA1;
            }
            o->wb6 = e->w2;
            o->wb8 = i++;
        }
    }
}
