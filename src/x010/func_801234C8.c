// FUNC 801234c8 576 X010
// MATCHING 801234c8 576
#include "TOBJ.H"

extern unsigned short D_800A6066;
extern unsigned char D_8009D2B0[], D_8009C93F[], D_8009C942;
extern unsigned int FUN_8001f9e0(void);
extern int FUN_8001fec0(TObj *);
extern void FUN_80026e0c(int, int);

void func_801234C8(TObj *o)
{
    TObj *p = (TObj *)o->d90;
    short t;
    short r;
    int u;

    switch (o->state) {
    case 0:
        o->animFrame = D_800A6066 & 1;
        t = p->h->p.whole - p->box0;
        if (o->h->p.whole < t) {
            o->h->p.whole = t;
            o->animFrame = 0;
        }
        t = p->h->p.whole + p->box0;
        if (t < o->h->p.whole) {
            o->h->p.whole = t;
            o->animFrame = 1;
        }
        r = FUN_8001f9e0() & 0xf;
        o->velH = r;
        if (o->animFrame & 1)
            o->velH = -r;
        o->velX = 0;
        o->velY = 0;
        FUN_80026e0c(0x10, 1);
        o->state++;
    case 1:
        FUN_8001fec0(o);
        t = o->h->p.whole - (p->h->p.whole - p->box0);
        if (p->box1 < (unsigned short)t)
            o->velH = -o->velH;
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velX += o->velH;
        o->velY -= 0x10;
        o->d84 = (o->d84 + 2) & 0xff;
        t = p->y.p.whole - p->box2;
        if (o->y.p.whole < t) {
            D_8009D2B0[0] = 0;
            *(volatile unsigned char *)&D_8009C942 = 0;
            D_8009C93F[0] = 0;
            *(volatile unsigned char *)&D_8009C942 = 0;
            o->y.p.whole = t;
            o->d30 = o->h->raw - p->h->raw;
            u = o->y.raw - p->y.raw;
            o->step = 4;
            o->state = 0;
            o->d34 = u;
        }
        break;
    }
}
