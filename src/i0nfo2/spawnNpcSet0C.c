// FUNC 800fa670 476 I0NFO2
// MATCHING 800fa670 476
#include "TOBJ.H"
#include "raw7.h"
typedef struct {
    short type, w2, frame, w6, w8, wA, wC, wE, w10, w12, x, y, sub, bc;
} SpawnA;
extern TObj *func_800183B8();
extern unsigned char D_8009CF01;
extern unsigned char D_8009CFC9;
extern TObj *SCRIPT_OBJECTS[];
extern SpawnA *D_800FFE20;
#define PI_O asm("s2")
#define PI_TBL asm("s4")
#define PI_LP asm("s5")
#define PI_ONE asm("s6")
#define PI_P asm("s0")
#define PI_K asm("s1")
void spawnNpcSet0C(TObj *o0)
{
    register TObj *o PI_O = o0;
    register SpawnA *tbl PI_TBL;
    register TObj **lp PI_LP;
    register int one PI_ONE;
    register SpawnA *p PI_P;
    register int k PI_K;
    TObj *n;
    short i;

    /* n lives in a0: seeded from the parameter and handed back to the
       (argument-less) allocator, so neither copy costs an instruction. */
    n = o0;
    tbl = D_800FFE20;
    o->b04 = 1;
    if (D_8009CFC9 != 0) {
        o->b04 = 3;
        return;
    }
    i = 0;
    p = tbl;
    if (p->type != 0xFF) {
    one = 1;
    lp = SCRIPT_OBJECTS;
    for (; p->type != 0xFF; p = &tbl[++i]) {
        k = i;
        if (k == one && D_8009CF01 != 0) continue;
        n = func_800183B8(n);
        if (n != 0) {
            n->active = one;
            do { } while (0);  /* keeps the active store ahead of the others */
            n->type = 0x30;
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
            lp[k] = n;
            n->d90 = (int)o;
        }
    }
    }
    o->step = 1;
}
