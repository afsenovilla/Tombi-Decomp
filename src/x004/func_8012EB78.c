// FUNC 8012eb78 296 X004
// MATCHING 8012eb78 296
#include "TOBJ.H"
typedef struct { unsigned short w0, w2, w4, w6, w8, wa, wc; } E;
extern E D_8013145C[];
extern unsigned short *D_800A6078;
extern unsigned short D_800A604E[];
extern TObj *FUN_800183b8(void);

void func_8012EB78(short i, short flag)
{
    E *p = &D_8013145C[i];
    TObj *n = FUN_800183b8();
    if (n == 0) return;
    n->active = 4;
    n->type = 0x32;
    n->animFrame = p->w0;
    n->subtype = p->w2;
    n->b0c = p->w4;
    n->b6b = p->wc;
    if (flag) {
        n->h->p.whole = p->w6;
        n->y.p.whole = p->w8;
        n->b.p.whole = p->wa;
        n->velV = 0;
        n->velH = 0;
    } else {
        n->velX = p->w6;
        n->velY = p->w8;
        n->h->p.whole = D_800A6078[1];
        n->y.p.whole = D_800A604E[0];
        n->b.p.whole = p->wa;
        n->velV = 3;
        n->velH = 3;
    }
}
