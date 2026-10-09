// FUNC 801191c4 524 X003
// MATCHING 801191c4 524
#include "TOBJ.H"

typedef struct {
    char p[0xb4];
    short b4, b6, b8, ba, bc, be, c0, c2, c4, c6, c8, ca, cc, ce, d0;
} E;

extern short D_8013589C[];
extern short D_1F800176[], D_1F800186;
extern int Rand(void);

void func_801191C4(TObj *o)
{
    E *e = (E *)o;

    switch (o->step) {
    case 0:
        o->step++;
        o->d->p.whole -= o->subtype * 4;
        break;
    case 1:
        o->step++;
        o->velX = 0x300;
        o->timer = 0x78;
        e->b4 = D_8013589C[0];
        e->b6 = D_8013589C[1];
        e->b8 = 0;
        e->bc = D_8013589C[2];
        e->be = D_8013589C[3];
        e->c0 = 0;
        e->c4 = D_8013589C[4];
        e->c6 = D_8013589C[5];
        e->c8 = 0;
        e->cc = D_8013589C[6];
        e->ce = D_8013589C[7];
        e->d0 = 0;
        o->a.raw = (D_1F800176[0] - 0xa0) << 16;
        { short r = (Rand() & 7) * 24 + 0x10; o->y.raw = (D_1F800186 - r) << 16; }
        o->velH = o->subtype << 7;
        break;
    case 2:
        o->h->raw += 0x60000 + (o->velH << 8);
        o->y.raw += -0x8000;
        if (o->timer != 0) {
            e->b4 += o->velX >> 8;
            o->timer--;
            e->bc += o->velX >> 8;
        }
        if (o->h->p.whole + e->c4 > D_1F800176[0] + 0x168) o->step = 1;
        break;
    }
}
