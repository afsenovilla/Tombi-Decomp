// FUNC 800fdca4 396 I0NFO2
// MATCHING 800fdca4 396
#include "TOBJ.H"
#include "raw7.h"
typedef struct {
    short type, w2, frame, w6, w8, wA, wC, wE, w10, w12, x, y, sub, bc;
} SpawnA;
extern TObj *func_800183B8();
extern TObj *D_8009F2D8[];
extern SpawnA *D_801008D8;
#define PI_PARENT 
#define PI_TBL asm("s3")
#define PI_LP asm("s4")
#define PI_TY asm("s5")
#define PI_FF asm("s6")
#define PI_P 
#define PI_I 
void func_800FDCA4(TObj *parent0)
{
    register TObj *parent PI_PARENT = parent0;
    register SpawnA *tbl PI_TBL;
    register TObj **lp PI_LP;
    register int ty PI_TY;
    register int ff PI_FF;
    register SpawnA *p PI_P;
    TObj *n;
    register short i PI_I;
    tbl = D_801008D8;
    i = 0;
    p = tbl;
    if (p->type != 0xFF) {
    ff = 0xFF; ty = 0x30; lp = D_8009F2D8;
    for (; p->type != ff; p = &tbl[++i]) {
        n = func_800183B8();
        if (n != 0) {
            n->active = 1;
            n->type = ty;
            n->b0a = 0xD;
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
            lp[i] = n;
            n->d90 = (int)parent;
        }
    }
    }
}
