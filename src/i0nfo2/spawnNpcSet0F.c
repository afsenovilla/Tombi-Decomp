// FUNC 800fc1c0 680 I0NFO2
// MATCHING 800fc1c0 680
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
extern unsigned char D_8009CFA1;
extern unsigned char D_8009CFA5;
extern unsigned char D_8009CF9F;
extern unsigned char D_8009CFA0;
extern TObj *SCRIPT_OBJECTS[];
extern SpawnB NPC_SET_0F_EXTRA_RECORDS[];
extern SpawnA *NPC_SET_0F_RECORDS;

void spawnNpcSet0F(TObj *parent0)
{
    register TObj *parent asm("s1") = parent0;
    SpawnA *tbl;
    SpawnA *p;
    SpawnB *e;
    TObj *o;
    TObj *n;
    short i;
    int i1, j;
    int m;
    unsigned short w;
    int ff;
    int ty;
    TObj **lp;

    if (D_8009CE0A != 0) {
        m = 1;
        if (D_8009CDF1 == 0xFF) {
            m = 2;
        }
    } else {
        m = 0;
    }
    tbl = NPC_SET_0F_RECORDS;
    D_8009CFA5 = m;
    do { } while (0);
    i1 = 0;
    p = tbl;
    if (p->type != 0xFF) do {
        ff = 0xFF; ty = 0x30; lp = SCRIPT_OBJECTS;
        n = func_800183B8();
        j = i1 + 1;
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
            lp[(short)i1] = n;
            n->d90 = (int)parent;
        }
        i1 = j;
    { register int m asm("v0"); m = (short)j * 28; p = (SpawnA *)((char *)tbl + m); } } while (p->type != ff);
    i = 0;
    for (e = NPC_SET_0F_EXTRA_RECORDS; e->type != 0xFF; e++) {
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
    D_8009CF9F = 0;
    D_8009CFA0 = 0;
}
