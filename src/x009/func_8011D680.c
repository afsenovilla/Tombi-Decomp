// FUNC 8011d680 352 X009
// MATCHING 8011d680 352
#include "TOBJ.H"

typedef struct { char p0; unsigned char b1; short w2; char p4[4]; unsigned char b8; char p9[5]; short we; } PL;
extern TObj *D_8009F0EC;
extern PL *D_8009C330;
extern int MulCosDup(int, short);
extern int MulNegSin(int, short);

void func_8011D680(TObj *o, int unused, int ang)
{
    TObj *g = D_8009F0EC;
    int a;
    PL *p;
    int t, w;

    o->d30 += g->velX;
    o->d34 += g->velY;
    if (o->animFrame & 1) {
        a = 0x1bf;
        a -= ang;
        a &= 0xff;
        o->h->p.whole = MulCosDup(a, D_8009C330->b1) + o->d30;
        a = MulNegSin(a, D_8009C330->b1);
        o->d84 = 0;
        o->d8c = 0x100 - (ang & 0xff);
        o->y.p.whole = a + o->d34;
    } else {
        a = ang + 0xc0;
        a &= 0xff;
        o->h->p.whole = MulCosDup(a, D_8009C330->b1) + o->d30;
        a = MulNegSin(a, D_8009C330->b1);
        o->d84 = 0;
        o->d8c = ang & 0xff;
        o->y.p.whole = a + o->d34;
    }
    p = D_8009C330;
    t = p->we;
    if (p->b8 != 0) {
        w = p->w2;
        p->we = t - w;
    } else {
        w = p->w2;
        p->we = t + w;
    }
}
