// FUNC 8012eb2c 1040 X003
// MATCHING 8012eb2c 1040
#include "TOBJ.H"
extern short D_8007A3F0[];
extern char D_80077CE8[];
extern void *D_8013A6B4[];
extern void *D_8013A6B8[];
extern void *D_8013A6BC[];
extern int FUN_8001fec0(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
extern void FUN_8001fe6c(TObj *);
extern short FUN_8004065c(TObj *, short, short, short);
extern short FUN_800408d8(TObj *, short, short);
extern short FUN_80040278(TObj *, short, short);
extern void FUN_8002b920(TObj *);

static __inline__ int hit(TObj *o)
{
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

void func_8012EB2C(TObj *o)
{
    int k;

    switch (o->state) {
    case 0:
        o->velV = -0x400;
        o->movetab = D_80077CE8;
        o->b9c = 1;
        o->d8c = 0;
        o->wac = 7;
        o->animFrame = 1 - o->w7a;
        o->state++;
        o->anim = D_8013A6B4[0];
        FUN_8001fe6c(o);
        break;
    case 1:
        FUN_8001fec0(o);
        FUN_8001fa88(o, o->w7a);
        k = 0xe;
        if (o->w7a & 1) k = -0xe;
        FUN_8004065c(o, o->h->p.whole + k, o->y.p.whole, o->w7a);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0 || FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10)) {
            o->velV = 0;
            o->b9c = 2;
            o->b69 = 0;
            o->state++;
        }
        break;
    case 2:
        FUN_8001fec0(o);
        FUN_8001fa88(o, o->w7a);
        k = 0xe;
        if (o->w7a & 1) k = -0xe;
        FUN_8004065c(o, o->h->p.whole + k, o->y.p.whole, o->w7a);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (hit(o) || o->velV > 0x580) {
            o->b9c = 0;
            o->state++;
            FUN_8002b920(o);
        }
        break;
    case 3:
        o->timer = 0x40;
        o->wac = 8;
        o->state++;
        o->anim = D_8013A6B8[0];
        FUN_8001fe6c(o);
        break;
    case 4:
        FUN_8001fec0(o);
        o->y.raw -= 0x8000;
        if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10) || --o->timer == -1) {
            o->timer = 300;
            o->state++;
        }
        break;
    case 5:
        o->ba7 += 2;
        o->y.raw += (short)(D_8007A3F0[o->ba7] << 2);
        FUN_8001fec0(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 9;
            o->state++;
            o->anim = D_8013A6BC[0];
            FUN_8001fe6c(o);
        }
        break;
    case 6:
        o->ba7 += 2;
        o->y.raw += (short)(D_8007A3F0[o->ba7] << 2);
        FUN_8001fec0(o);
        if (--o->timer == -1) {
            o->active = 1;
            o->b04 = 1;
            o->step = 1;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
