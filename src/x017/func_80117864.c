// FUNC 80117864 392 X017
// MATCHING 80117864 392
#include "TOBJ.H"
typedef struct { void **p; int x, y; } AT;
extern unsigned char D_8009C942, D_8009C93F, D_8009CFD6;
extern signed char D_8009D2B0;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern AT D_80119990[];
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_80026e0c(int, int);

void func_80117864(TObj *o)
{
    switch (o->state) {
    case 0:
        D_8009C942 = 1;
        D_8009C93F = 1;
        D_8009D2B0 = 2;
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        o->anim = D_80119990[o->subtype].p[1];
        FUN_8001fe94(o, 0);
        o->animFrame = 1;
        o->timer = 60;
        o->state++;
        break;
    case 1:
        o->h->p.whole -= 2;
        FUN_8001fec0(o);
        if (--o->timer > 0) break;
        FUN_80026e0c(0x3c, 1);
        D_8009CFD6 = 1;
        D_800A603C = 1;
        D_800A603D = 0x31;
        D_800A603E = 0;
        FUN_80026e0c(0x3c, 1);
        o->state++;
        break;
    case 2:
        o->h->p.whole -= 2;
        FUN_8001fec0(o);
        break;
    }
}
