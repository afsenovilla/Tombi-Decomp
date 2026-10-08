// FUNC 80108168 464 X018
// MATCHING 80108168 464
#include "TOBJ.H"
#define B(o, k) (*(unsigned char *)((char *)(o) + (k)))
extern TObj *DAT_8009c330;
extern int DAT_8009c960[];
extern unsigned char DAT_8009cda2, DAT_8009c93a;
extern void FUN_800eeb5c(TObj *, int);
extern void FUN_8001fec0(TObj *);

void func_80108168(TObj *o)
{
    unsigned char t;
    unsigned short u;
    switch (o->state) {
    case 0:
        B(DAT_8009c330, 8) = o->active;
//PS
        u = o->animFrame;
        o->velX = 0x5a;
        o->active = 2;
        o->d8c = 0;
        o->velY = 0;
        o->b9c = 0;
        o->b9d = 0;
        o->b9e = 0;
        o->b9f = 0;
        B(o, 0xad) = 0;
        o->b69 = 0;
        *(signed char *)((char*)o+0xf) = -20;
        B(o, 0xa2) = 2;
        o->animFrame = u & 1;
        FUN_800eeb5c(o, 0x2b);
        o->state++;
    case 1:
        FUN_8001fec0(o);
        if (DAT_8009cda2 == 0)
            o->state++;
        break;
    case 2:
        FUN_8001fec0(o);
        o->d->p.whole += 5;
        o->velX -= 5;
        if (DAT_8009c960[0] == 0x40001) {
            o->y.raw += -0x60000;
            if (o->y.p.whole < -0x20f)
                o->y.p.whole = -0x20f;
        }
        if (o->velX == 0) {
            *(signed char *)&o->b0f = -8;
            t = B(DAT_8009c330, 8);
            o->b9c = 0;
            o->active = t;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            DAT_8009c330->timer = 0;
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            o->timer = 0;
            DAT_8009c93a = 1;
        }
        break;
    }
}
