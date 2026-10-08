// FUNC 80108ad0 748 X006
// MATCHING 80108ad0 748
#include "TOBJ.H"
#define B(o, n) (((unsigned char *)(o))[n])
extern TObj *D_8009C330;
extern int D_8009C960[];
extern unsigned short DAT_1f8001c8;
extern void PlayerSetAnimIfChanged(TObj *, int);
extern int AnimAdvance(TObj *);

void func_80108AD0(TObj *o)
{
    unsigned short f;
    switch (o->state) {
    case 0:
        B(D_8009C330, 8) = o->active;
        B(D_8009C330, 9) = 0;
        o->velX = 0xb4;
        o->state++;
        if (D_8009C960[0] == 0x40001) {
            B(D_8009C330, 9) = 1;
            o->velX = 0x78;
            o->state = 2;
        }
        if (*(unsigned short *)D_8009C960 == 8) {
            o->velX = 0x12c;
            o->state = 1;
        }
        f = o->animFrame;
        o->active = 2;
        o->d8c = 0;
        o->velY = 0;
        o->b9c = 0;
        o->b9d = 0;
        o->b9e = 0;
        o->b9f = 0;
        B(o, 0xad) = 0;
        o->b69 = 0;
        *(signed char *)&o->b0f = -20;
        B(o, 0xa3) = 2;
        o->animFrame = f & 1;
        PlayerSetAnimIfChanged(o, 0x2c);
    case 1:
        AnimAdvance(o);
        if (DAT_1f8001c8 & 1)
            o->d->p.whole += 2;
        else
            o->d->p.whole -= 2;
        o->velX -= 2;
        if (o->velX <= 0)
            o->state = 9;
        break;
    case 2:
        AnimAdvance(o);
        o->d->p.whole -= 1;
        o->velX -= 1;
        if (o->velX <= 0) {
            o->velX = 0x18;
            o->state = 3;
        }
        break;
    case 3:
        AnimAdvance(o);
        o->y.p.whole += 2;
        o->velX -= 2;
        if (o->velX <= 0) {
            o->velX = 0x18;
            o->state = 4;
        }
        break;
    case 4:
        AnimAdvance(o);
        o->d->p.whole -= 1;
        o->velX -= 1;
        if (o->velX <= 0) {
            o->velX = 0x18;
            o->state = 3;
        }
        if (o->d->p.whole < 2000)
            o->state = 8;
        break;
    case 8:
        AnimAdvance(o);
        o->d->p.whole -= 1;
        break;
    case 9:
        AnimAdvance(o);
        *(signed char *)&o->b0f = -8;
        o->active = B(D_8009C330, 8);
        o->b9c = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        D_8009C330->timer = 0;
        break;
    }
}
