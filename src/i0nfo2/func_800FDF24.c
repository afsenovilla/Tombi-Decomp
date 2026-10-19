// FUNC 800fdf24 1260 I0NFO2
// MATCHING 800fdf24 1260
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
extern unsigned char D_8009CE56[];
extern unsigned char D_8009CFCA;
extern TObj *D_8009F2D8[];
extern SpawnB D_801008DC[];
extern SpawnA *D_80100B00;

#define PI_PARENT 
#define PI_TBL 
#define PI_FF asm("s4")
#define PI_D asm("s6")
#define PI_LP asm("s5")
#define PI_CE asm("s7")
#define PI_P 
#define PI_E 
#define PI_I 
#define PI_K 
#define PI_I1 
void func_800FDF24(TObj *parent0)
{
    register TObj *parent PI_PARENT = parent0;
    register SpawnA *tbl PI_TBL;
    register int ff PI_FF;
    register int d PI_D;
    register TObj **lp PI_LP;
    register unsigned char *ce PI_CE;
    register SpawnA *p PI_P;
    register SpawnB *e PI_E;
    TObj *n;
    TObj *o;
    register short i PI_I;
    register int k PI_K;
    register int i1 PI_I1;
    short t;

    tbl = D_80100B00;
    i = 0;
    p = tbl;
    if (p->type != 0xFF) {
    ce = D_8009CE56;
    ff = 0xFF;
    d = 0xD;
    lp = D_8009F2D8;
    for (; p->type != ff; p = &tbl[++i]) {
        k = i;
        if (k == 5 && D_8009CFCA != 4) continue;
        if (k == 7 && (ce[0] == ff || ce[0x179] == 0)) continue;
        if (k == 8 && D_8009CE56[0] == ff) continue;
        if (k == 9 && (ce[0] == ff || ce[0x179] == 0)) continue;
        n = func_800183B8();
        if (n != 0) {
            n->active = 1;
            t = p->w2;
            if (t == ff) {
                n->type = 0x30;
                n->b0a = d;
                n->animFrame = p->frame;
                n->subtype = p->sub;
                n->b0c = p->bc;
                n->_pad0e[0] = p->type;
                n->a.p.whole = p->x;
                n->y.p.whole = p->y;
                n->b.p.whole = 0;
                n->wb4 = p->w6;
                n->wb6 = p->w2;
            } else if (t & 0x80) {
                n->type = 0x2E;
                n->b0a = d;
                n->animFrame = p->frame;
                n->subtype = p->sub;
                n->b0c = p->bc;
                n->_pad0e[0] = p->type;
                n->a.p.whole = p->x;
                n->y.p.whole = p->y;
                n->b.p.whole = 0;
                n->wb4 = p->w6;
                n->wb6 = p->w2 & 0x7F;
            } else if (t & 0x40) {
                n->type = 0x2D;
                n->b0a = d;
                n->animFrame = p->frame;
                n->subtype = p->sub;
                n->b0c = p->bc;
                n->_pad0e[0] = p->type & 0x3F;
                n->a.p.whole = p->x;
                n->y.p.whole = p->y;
                n->b.p.whole = 0;
                n->wb4 = p->w6;
                n->wb6 = p->w2 & 0x3F;
            } else {
                goto plain;
            }
            n->wb8 = p->w12;
            n->wba = p->w10;
            n->w74 = p->w8;
            n->w76 = p->wA;
            n->w78 = p->wC;
            n->w7a = p->wE;
            lp[k] = n;
            n->d90 = (int)parent;
            continue;
        plain:
                n->type = 0x30;
                n->b0a = d;
                n->animFrame = p->frame;
                n->subtype = p->sub;
                n->b0c = p->bc;
                n->_pad0e[0] = p->type;
                n->a.p.whole = p->x;
                n->y.p.whole = p->y;
                n->b.p.whole = 0;
                n->wb4 = p->w6;
                n->wb6 = p->w2;
                n->wb8 = p->w12;
                n->wba = p->w10;
                n->w74 = p->w8;
                n->w76 = p->wA;
                n->w78 = p->wC;
                n->w7a = p->wE;
                lp[k] = n;
                n->d90 = (int)parent;
                if (k < 10) {
                    if (k >= 7) {
                        n->active = 2;
                    }
                }
                continue;
        }
    }
    }
    for (i = 0; (e = &D_801008DC[i])->type != 0xFF; i++) {
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
            o->wb4 = e->w6;
            o->wb6 = e->w2;
            o->wb8 = e->w4;
        }
    }
}
