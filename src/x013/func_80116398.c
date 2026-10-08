// FUNC 80116398 840 X013
// MATCHING 80116398 840
#include "TOBJ.H"
typedef struct {
    TObj t;
    unsigned char pad[0xf2 - 0xc0];
    short wf2;
} PL;
extern PL D_800A6038;
extern unsigned char D_8009CFF9, D_8009C975;
extern signed char D_8009D2B0;
extern int FUN_800202b4(TObj *);
extern short FUN_80040278(TObj *, short, short);
extern void FUN_800eea7c(TObj *, short, short);
extern void FUN_80049ca4(TObj *, int);
extern void FUN_8001fec0(TObj *);

void func_80116398(TObj *o)
{
    PL *p;

    FUN_800202b4(o);
    p = &D_800A6038;
    switch (o->state) {
    case 0:
        if ((*(int *)&p->t.b04 & 0xffffff) != 0x30405) break;
        o->h->p.whole = p->t.h->p.whole;
        p->t.h->p.whole = o->h->p.whole;
        p->t.y.p.whole = o->y.p.whole - 0x14;
        *(signed char *)&o->b0f = -10;
        p->t.active = 5;
        p->t.visible = 1;
        p->t.b04 = 5;
        p->t.step = 0x41;
        p->t.state = 0;
        FUN_800eea7c(&p->t, 0x2c, 0);
        FUN_80049ca4(o, 1);
        o->velV = 0x500;
        o->state++;
        break;
    case 1:
        o->velV -= 8;
        if (o->velV < 0x100) o->velV = 0x100;
        o->y.raw += o->velV << 8;
        p->t.y.raw += o->velV << 8;
        if (o->y.p.whole < p->wf2 - 0x1e) break;
        if (!FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) break;
        o->timer = 0x1e;
        o->state++;
        FUN_80049ca4(o, 0);
        break;
    case 2:
        if (--o->timer != -1) break;
        o->state++;
        break;
    case 3:
        p->t.y.raw += 0x8000;
        if (!FUN_80040278(&p->t, p->t.h->p.whole, p->t.y.p.whole + 0x14)) break;
        o->state++;
        FUN_80049ca4(o, 0);
        break;
    case 4:
        o->timer = 0x1e;
        p->t.animFrame = 1;
        FUN_800eea7c(&p->t, 1, 0);
        o->state++;
        D_8009CFF9 = 0;
        o->active = 2;
        break;
    case 5:
        FUN_8001fec0(&p->t);
        p->t.h->p.whole--;
        if (--o->timer != -1) break;
        o->timer = 0x1e;
        o->state++;
        p->t.animFrame = 1;
        FUN_800eea7c(&p->t, 0, 0);
        break;
    case 6:
        if (--o->timer != -1) break;
        o->state++;
        D_8009D2B0 = 0;
        p->t.b04 = 5;
        p->t.step = 0x65;
        p->t.state = 0;
        break;
    case 7:
        if (D_8009C975 == 1) o->b04 = 3;
        break;
    }
}
