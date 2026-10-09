// FUNC 80137afc 656 X001
// MATCHING 80137afc 656
#include "TOBJ.H"

extern unsigned short D_800A6066;
extern signed char D_8009D2B0[];
extern unsigned char D_8009C942, D_8009C93F[];
extern int Rand(void);
extern int AnimAdvance(TObj *o);
extern void FUN_80026e0c(int, int);

void func_80137AFC(TObj *o)
{
    TObj *p = (TObj *)o->d90;
    unsigned short t;

    switch (o->state) {
    case 0:
        o->animFrame = D_800A6066 & 1;
        t = p->h->p.whole - p->box0;
        if (o->h->p.whole < (short)t) {
            o->h->p.whole = t;
            o->animFrame = 0;
        }
        t = p->h->p.whole + p->box0;
        if ((short)t < o->h->p.whole) {
            o->h->p.whole = t;
            o->animFrame = 1;
        }
        o->velH = Rand() & 0xf;
        if (o->animFrame & 1) o->velH = -o->velH;
        o->velX = 0;
        o->velY = 0;
        FUN_80026e0c(0x10, 1);
        o->state++;
    case 1:
        AnimAdvance(o);
        t = o->h->p.whole - (p->h->p.whole - p->box0);
        if (t > p->box1) o->velH = -o->velH;
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velX += o->velH;
        o->velY -= 0x10;
        t = p->h->p.whole - p->box0;
        if (o->h->p.whole < (short)t) {
            o->h->p.whole = t;
        } else {
            t = p->h->p.whole + p->box0;
            if ((short)t < o->h->p.whole) o->h->p.whole = t;
        }
        o->d84 = (o->d84 + 2) & 0xff;
        t = p->y.p.whole - p->box2;
        if (o->y.p.whole < (short)t) {
            {
                unsigned char *q = &D_8009C942;
                D_8009D2B0[0] = 0;
                *q = 0;
                D_8009C93F[0] = 0;
                *q = 0;
            }
            o->y.p.whole = t;
            o->d30 = o->h->raw - p->h->raw;
            o->d34 = o->y.raw - p->y.raw;
            o->step = 4;
            o->state = 0;
        }
        break;
    }
}
