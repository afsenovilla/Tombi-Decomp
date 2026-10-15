// FUNC 8012eca0 372 X004
// MATCHING 8012eca0 372
#include "gcd.h"
#include "TOBJ.H"
typedef struct { unsigned short w0, w2, w4, w6, w8, wa, wc; } E;
typedef struct { unsigned short w0, w2, w4, w6, w8, wa; } F;
extern F D_801314A4;
extern E D_8013145C[];
extern TObj *allocObjectLayer2(void);

void spawnBigKeyPaintingPieces(void)
{
    TObj *n;
    short i;
    E *p;

    {
    TObj *m = allocObjectLayer2();
    if (m) {
        m->active = 3;
        m->type = 0x32;
        m->animFrame = D_801314A4.w0;
        m->subtype = D_801314A4.w2;
        m->b0c = D_801314A4.w4;
        m->a.p.whole = D_801314A4.w6;
        m->y.p.whole = D_801314A4.w8;
        m->b.p.whole = D_801314A4.wa;
    }
    }
    for (i = 0; i < 5; i++) {
        if (((unsigned char*)&GCD.bcf24)[i] == 0) continue;
        p = &D_8013145C[i];
        n = allocObjectLayer2();
        if (n == 0) continue;
        n->active = 4;
        n->type = 0x32;
        n->animFrame = p->w0;
        n->subtype = p->w2;
        n->b0c = p->w4;
        n->b6b = p->wc;
        n->h->p.whole = p->w6;
        n->y.p.whole = p->w8;
        n->b.p.whole = p->wa;
        n->velV = 0;
        n->velH = 0;
    }
}
