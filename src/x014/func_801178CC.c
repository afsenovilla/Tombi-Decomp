// FUNC 801178cc 476 X014
// MATCHING 801178cc 476
#include "TOBJ.H"
extern TObj *FUN_80018448(void);
extern int Rand(void);

#define S16(o, off) (*(short *)((char *)(o) + (off)))

void func_801178CC(TObj *o, short dx, short dy, short dz)
{
    short t[6] = { -0xc8, -0xb4, -0xbe, -0xc8, -0xb4, -0xbe };
    TObj *n;
    int i;

    for (i = 0; i < 12; i++) {
        n = FUN_80018448();
        if (n == 0) continue;
        n->type = 0x61;
        n->b0c = i >> 1;
        n->active = 1;
        n->subtype = 0;
        n->b0a = 0x14;
        n->h->p.whole = o->h->p.whole + dx;
        n->y.p.whole = o->y.p.whole + dy;
        n->d->p.whole = o->d->p.whole + dz;
        n->animFrame = i * 0x15;
        n->d84 = (Rand() & 3) << 8;
        n->d88 = 0x100;
        n->d8c = 0;
        n->w76 = t[n->b0c];
        n->d30 = 0xf0;
        n->timer = (Rand() & 0xf) + 1;
        n->d90 = (int)o;
        *(int *)&n->w5c = dx;
        n->d60 = dy;
        n->d64 = dz;
        n->wb4 = -8;
        n->b0b = 1;
        n->b0f = 0;
        n->wb6 = 0;
        n->wb8 = 0;
        n->wbc = 8;
        S16(n, 0xbe) = 0;
        S16(n, 0xc0) = 0;
        S16(n, 0xc4) = 0;
        S16(n, 0xc6) = 0;
        S16(n, 0xc8) = 0;
    }
}
