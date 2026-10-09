// FUNC 80126890 1104 X009
// MATCHING 80126890 1104
#include "TOBJ.H"

extern void *D_8012EE84[], *D_8012EE94[], *D_8012EE80[];
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001e4f0(int);
extern short FUN_8004065c(TObj *, short, short, short);
extern short FUN_80040278(TObj *, short, short);

#define WALLCHK(o, r)                                                       \
    {                                                                       \
        t = (o)->b9d;                                                       \
        af = (o)->animFrame;                                                \
        if ((t & 2) && af == (t & 1))                                       \
            r = 1;                                                          \
        else {                                                              \
            f = af;                                                         \
            if (f)                                                          \
                d = -0x10;                                                  \
            else                                                            \
                d = 0x10;                                                   \
            r = FUN_8004065c(o, (o)->h->p.whole + d, (o)->y.p.whole + 0x30, f); \
        }                                                                   \
    }

void func_80126890(TObj *o)
{
    unsigned char t;
    unsigned short af;
    int r;
    short f, d;

    switch (o->state) {
    case 0:
        o->active = 3;
        o->state++;
        if (o->animFrame) {
            o->velH = -0x280;
            o->velX = 0x200;
        } else {
            o->velH = 0x280;
            o->velX = -0x200;
        }
        o->timer = 0x20;
        o->wac = 0;
        o->anim = D_8012EE84[0];
        FUN_8001fe6c(o);
        o->b0a = 0;
        o->d8c = 0;
        o->w9a = 0;
        FUN_8001e4f0(0x9c);
        break;
    case 1:
        FUN_8001fec0(o);
        o->h->raw += o->velH << 8;
        WALLCHK(o, r);
        if (r) {
            o->state = 3;
            o->velV = 0x200;
            o->velY = -0x10;
            o->timer = 0;
            o->wac = 4;
            o->velH += o->velX;
            o->anim = D_8012EE94[0];
            FUN_8001fe6c(o);
            break;
        }
        FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x30);
        if (--o->timer == -1) {
            o->wac = 4;
            o->state++;
            o->anim = D_8012EE94[0];
            FUN_8001fe6c(o);
            o->velH += o->velX;
        }
        break;
    case 2:
        o->h->raw += o->velH << 8;
        WALLCHK(o, r);
        if (!r)
            FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x30);
        if (FUN_8001fec0(o))
            o->state = 4;
        break;
    case 3:
        if (FUN_8001fec0(o)) {
            o->wac = 0;
            o->anim = D_8012EE80[0];
            FUN_8001fe6c(o);
        }
        o->h->raw -= o->velH << 8;
        WALLCHK(o, r);
        o->y.raw -= o->velV << 8;
        o->velV += o->velY;
        if (o->velV < -0x1ff)
            o->state = 4;
        FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x30);
        break;
    case 4:
        o->timer = 0x20;
        o->wac = 0;
        o->state++;
        o->anim = D_8012EE80[0];
        FUN_8001fe6c(o);
        break;
    case 5:
        FUN_8001fec0(o);
        if (--o->timer == -1) {
            o->active = 1;
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->animFrame = 1 - o->animFrame;
        }
        break;
    }
}
