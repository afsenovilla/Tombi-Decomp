// FUNC 8012f234 380 X003
// MATCHING 8012f234 380
#include "TOBJ.H"
extern void *D_8013A70C[];
extern unsigned char D_800A603E, D_800A60E4;
extern void FUN_80026bfc(int, int);
extern void FUN_8001fe6c(TObj *);
extern short FUN_80040278(TObj *, short, short);

static __inline__ short land(TObj *o)
{
    if (o->b69 == 1 || FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

void func_8012F234(TObj *o)
{
    switch (o->state) {
    case 0:
        FUN_80026bfc(1, 6);
        *(signed char *)&o->b0f = -7;
        o->b68 = 0;
        o->d8c = 0;
        o->velV = 0;
        o->wac = 0x1d;
        o->state++;
        o->anim = D_8013A70C[0];
        FUN_8001fe6c(o);
        break;
    case 1:
        o->state++;
        o->velV += 0x20;
        if (o->velV > 0x300) o->velV = 0x300;
        o->y.raw += o->velV << 8;
        land(o);
        break;
    case 2:
        o->state++;
        D_800A603E = 2;
        D_800A60E4 = 3;
    case 3:
        o->velV += 0x20;
        if (o->velV > 0x300) o->velV = 0x300;
        o->y.raw += o->velV << 8;
        land(o);
        break;
    case 4:
        o->state++;
        break;
    case 5:
        break;
    }
}
