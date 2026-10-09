// FUNC 8012a290 828 X001
// MATCHING 8012a290 828
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned short D_800A6066;
extern short D_800A60B6;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern void *D_8013FC88[], *D_8013FC8C[], *D_8013FCCC[];
extern unsigned char D_8013C7C8[];
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern int Rand(void);
extern void FUN_8001faf4(TObj *);
extern int FUN_800408d8(TObj *, int, int);
extern void FUN_8004258c(TObj *, int);

void func_8012A290(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b9c = 1;
        o->state++;
    case 1:
        if (AnimAdvance(o)) {
            o->velV = -0x400;
            o->wac = 6;
            o->state++;
            o->anim = D_8013FC88[0];
            AnimLoadDuration(o);
        }
        D_800A6038.b69 = 1;
        D_800A6038.h->p.whole = o->h->p.whole;
        {
            short t = D_800A6038.y.p.whole - 0x18;
            o->velY = o->y.p.whole - t;
        }
        if (o->velY != 0) {
            if (o->velY < 0) D_800A6038.y.p.whole--;
            else D_800A6038.y.p.whole++;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->velV += 0x10;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) o->state++;
        else if ((short)FUN_800408d8(o, o->h->p.whole, (short)(o->y.p.whole - 0x10))) o->velV = 0;
        D_800A6038.b69 = 1;
        D_800A6038.h->p.whole = o->h->p.whole;
        D_800A6038.y.p.whole = o->y.p.whole + (D_800A6038.box3 - D_800A6038.box2);
        break;
    case 3: {
        TObj *p = &D_800A6038;
        *(signed char *)&o->b0f = -9;
        o->b6a = 0;
        D_800A6066 = 2;
        D_800A60B6 = 0;
        p->active = 2;
        D_800A603C = 2;
        D_800A603D = 0;
        D_800A603E = 1;
        FUN_8004258c(p, 1);
        D_800A60B6 = 0x200;
        o->wac = 7;
        o->state++;
        o->anim = D_8013FC8C[0];
        AnimLoadDuration(o);
        break;
    }
    case 4:
        if (AnimAdvance(o)) {
            o->active = 1;
            o->state++;
        }
        break;
    case 5:
        o->state++;
        if (D_8013C7C8[Rand() & 0xf]) o->timer = 0x50;
        else o->timer = 0x32;
        o->wac = 0x17;
        o->anim = D_8013FCCC[0];
        AnimLoadDuration(o);
        break;
    case 6:
        FUN_8001faf4(o);
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->timer = 0x80;
            o->step = 7;
            o->state = 0;
        }
        break;
    }
}
