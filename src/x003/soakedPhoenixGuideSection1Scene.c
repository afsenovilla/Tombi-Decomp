// FUNC 80132960 1056 X003
// MATCHING 80132960 1056
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
#define P PLAYER
extern TObj *D_8009F3D4;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern void playerSetAnimIfChangedAt(TObj *, int, int);

void soakedPhoenixGuideSection1Scene(TObj *o)
{
    TObj *e = D_8009F3D4;

    switch (o->state) {
    case 0: {
        unsigned char *f = &GCD.bd00a;
        short r;
        if (*f == 1) {
            ((unsigned char*)&GVARS.b3f)[0] = 1;
            ((unsigned char*)&GVARS.b42)[0] = 1;
            P.animFrame = 0;
            r = e->h->p.whole < P.h->p.whole;
            D_800A603C[0] = 5;
            D_800A603D[0] = 0x64;
            D_800A603E[0] = 0;
            *f = 2;
            P.animFrame = r;
            o->state++;
            e->step = 1;
            e->state = 0;
        }
        break;
    }
    case 1:
        o->state++;
    case 2:
        if (GCD.bcef0 == 1) {
            playerSetAnimIfChangedAt(&P, 0x14, 0);
            o->w08 = 0x14;
            o->state++;
        }
        break;
    case 3:
        if (--o->w08 <= 0) {
            playerSetAnimIfChangedAt(&P, 0x15, 0);
            if (e->h->p.whole < P.h->p.whole) P.velX = -0x200;
            else P.velX = 0x200;
            P.velY = -0x200;
            P.b9c = 1;
            o->w08 = 0x10;
            o->state++;
        }
        break;
    case 4:
        P.d->p.whole += 5;
        P.y.raw += P.velY << 8;
        P.velY += 0x10;
        P.h->raw += P.velX << 8;
        if (P.velX < 0) {
            if (P.h->p.whole < e->h->p.whole) P.h->p.whole = e->h->p.whole;
        } else {
            if (e->h->p.whole < P.h->p.whole) P.h->p.whole = e->h->p.whole;
        }
        if (P.velY > 0) {
            P.b9c = 2;
            o->state++;
        }
        if (P.d->p.whole >= 0x88) P.d->p.whole = 0x87;
        break;
    case 5:
        P.d->p.whole += 5;
        P.y.raw += P.velY << 8;
        P.velY += 0x10;
        if (P.d->p.whole >= 0x88) P.d->p.whole = 0x87;
        P.h->raw += P.velX << 8;
        if (P.velX < 0) {
            if (P.h->p.whole < e->h->p.whole) P.h->p.whole = e->h->p.whole;
        } else {
            if (e->h->p.whole < P.h->p.whole) P.h->p.whole = e->h->p.whole;
        }
        if (e->y.p.whole - 0x10 < P.y.p.whole) {
            playerSetAnimIfChangedAt(&P, 0xd, 0);
            P.animFrame = 0;
            P.h->p.whole = e->h->p.whole;
            P.y.p.whole = e->y.p.whole;
            e->step = 2;
            e->state = 0;
            o->state++;
        }
        break;
    case 6:
        break;
    }
}
