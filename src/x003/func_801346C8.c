// FUNC 801346c8 1468 X003
// MATCHING 801346c8 1468
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **p; int a, b; } T12;
extern T12 D_80136148[];
extern TObj D_800A6038;
#define P D_800A6038
extern unsigned char D_8009CDAB, D_8009CE3B, D_8009CFCA, D_8009D13E;
extern unsigned char D_8009C942[], D_8009C93E[], D_8009CFCD[];
extern signed char D_8009D2B0[];
extern unsigned char D_800A603C, D_800A603D;
extern short D_800A60EA[];
extern short D_1F8001C6;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, V6 *);
extern short FUN_80040278(TObj *, int, int);
extern void FUN_80026c50(int, int, int);
extern void FUN_8005a9a4(int, int);

void func_801346C8(TObj *o)
{
    V6 v;
    unsigned char s;

    switch (o->state) {
    case 0:
        s = D_8009CDAB;
        if (s != 1) break;
        if (P.d->p.whole != 0x5a) break;
        if ((unsigned short)(P.h->p.whole - 0xc58) >= 0x14) break;
        if ((unsigned short)(P.y.p.whole + 0x6f4) >= 0xa) break;
        if (D_800A603C != 5) break;
        if (D_800A603D != 0xb) break;
        v = *(V6 *)&o->a;
        D_8009C942[0] = 1;
        D_8009C93E[0] = 1;
        D_8009D2B0[0] = 2;
        D_8009CFCD[0] = 1;
        o->wbc = o->animFrame;
        o->animFrame = 1;
        o->anim = D_80136148[o->subtype].p[1];
        AnimLoadDuration(o);
        if (D_8009CE3B == s) {
            o->d90 = FUN_8002dcc8(5, 0, &v);
            o->state = 10;
        } else {
            o->d90 = FUN_8002dcc8(5, 0, &v);
            o->state = 1;
        }
        break;
    case 1:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->anim = D_80136148[o->subtype].p[0];
        AnimLoadDuration(o);
        o->velX = -0x180;
        o->velY = -0x400;
        o->state++;
        break;
        }
    case 2:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (o->velY > 0) o->state++;
        if (!o->visible) o->state = 9;
        break;
    case 3:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (FUN_80040278(o, o->h->p.whole, o->y.p.whole)) {
            o->anim = D_80136148[o->subtype].p[0];
            AnimLoadDuration(o);
            o->velX = -0x180;
            o->velY = -0x400;
            o->state = 2;
        }
        if (!o->visible) o->state = 9;
        break;
    case 9:
        D_8009C942[0] = 0;
        D_8009C93E[0] = 0;
        D_800A60EA[0] = 0;
        o->animFrame = o->wbc;
        { unsigned char *c = &D_8009CFCA; *c = *c + 1; }
        D_1F8001C6 = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    case 10:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        if (D_8009D13E == 0) {
            o->anim = D_80136148[o->subtype].p[0];
            AnimLoadDuration(o);
            v = *(V6 *)&o->a;
            o->d90 = FUN_8002dcc8(5, 2, &v);
            o->state = 11;
        } else {
            o->anim = D_80136148[o->subtype].p[0];
            AnimLoadDuration(o);
            v = *(V6 *)&o->a;
            o->d90 = FUN_8002dcc8(5, 1, &v);
            o->state = 12;
        }
        break;
        }
    case 11:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state = 14;
        break;
        }
    case 12:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        FUN_80026c50(0x37, 1, 1);
        FUN_80026c50(0x37, 1, 1);
        o->state = 14;
        break;
        }
    case 14:
        o->timer = 200;
        FUN_8005a9a4(0x97, 0);
        o->state++;
        break;
    case 15:
        if (--o->timer <= 0) o->state = 1;
        break;
    }
}
