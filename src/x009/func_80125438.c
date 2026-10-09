// FUNC 80125438 1536 X009
// MATCHING 80125438 1536
#include "TOBJ.H"

extern void *D_8012EE84[], *D_8012EE94[], *D_8012EE80[];
extern char D_80077D0C[], D_80077D00[];
extern unsigned short D_1F80016A, D_1F800176;
extern short D_8007A5F0[];
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
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

#define PAR(o) (*(TObj **)&(o)->wa8)

void func_80125438(TObj *o)
{
    unsigned char t;
    unsigned short af;
    int r;
    short f, d;
    TObj *p;

    switch (o->state) {
    case 0:
        o->movetab = D_80077D0C;
        o->velV = 0x180;
        o->velY = -0x18;
        o->wac = 0;
        o->state++;
        o->anim = D_8012EE84[0];
        FUN_8001fe6c(o);
        o->d30 = PAR(o)->h->p.whole;
        o->d34 = PAR(o)->y.p.whole;
        break;
    case 1:
        o->state++;
        o->timer = 0;
        if ((short)(D_1F80016A - o->h->p.whole) >= 0)
            o->animFrame = 1;
        else
            o->animFrame = 0;
    case 2:
        FUN_8001fec0(o);
        if ((unsigned short)(o->h->p.whole - D_1F800176 - 0x30) < 0x110) {
            FUN_8001fa88(o, o->animFrame);
            {
                int q;
                WALLCHK(o, q);
                if (q) o->animFrame = 1 - o->animFrame;
            }
            o->y.raw += o->velV << 8;
            if (o->velV >= -0x17f) o->velV += o->velY;
            FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x30);
        } else {
            o->wac = 4;
            o->state++;
            o->anim = D_8012EE94[0];
            FUN_8001fe6c(o);
            o->movetab = D_80077D00;
        }
        break;
    case 3:
        FUN_8001fa88(o, o->animFrame);
        WALLCHK(o, r);
        o->y.raw += o->velV << 8;
        FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x30);
        if (FUN_8001fec0(o)) o->state++;
        break;
    case 4:
        o->state++;
        o->wac = 0;
        o->anim = D_8012EE80[0];
        FUN_8001fe6c(o);
        o->velV = 0x100;
        o->d88 = 0;
        o->animFrame = 1 - o->animFrame;
        FUN_8001e4f0(0x97);
        break;
    case 5:
        FUN_8001fec0(o);
        o->y.raw += (D_8007A5F0[o->d88] * o->velV) >> 4;
        FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x30);
        o->d88 = (o->d88 + 2) & 0xff;
        break;
    }
    {
        TObj *q = PAR(o);
        if (*(unsigned short *)&q->b04 == 0x102) {
            r = -1;
        } else if (*(unsigned short *)&q->b04 == 0x202) {
            if ((short)(q->h->p.whole - o->h->p.whole) < 0) {
                if (q->animFrame & 1) {
                m2:
                    r = -2;
                } else {
                    r = 1;
                }
            } else {
                if (!(q->animFrame & 1)) goto m2;
                r = 1;
            }
        } else {
            r = 0;
        }
    }
    switch (r) {
    case -2:
        o->step = 0;
        o->state = 0;
        break;
    case -1:
        {
            short dy, dz, dx, ex;
            p = PAR(o);
            dz = p->y.p.whole - o->d34;
            dy = o->y.p.whole - p->y.p.whole;
            dx = p->h->p.whole - o->d30;
            ex = o->h->p.whole - p->h->p.whole;
            if (ex > 0 && dx > 0)
                o->h->p.whole += dx >> 1;
            else if (ex < 0 && dx < 0)
                o->h->p.whole += dx >> 1;
            if (dy >= -0x4f)
                o->y.p.whole -= dz >> 1;
            else
                o->y.p.whole += dz >> 1;
            WALLCHK(o, r);
            FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x30);
            o->d30 = PAR(o)->h->p.whole;
            o->d34 = PAR(o)->y.p.whole;
        }
        break;
    case 0:
        o->step = 0;
        o->state = 0;
        break;
    case 1:
        o->step = 4;
        o->state = 0;
        break;
    }
}
