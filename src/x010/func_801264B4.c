// FUNC 801264b4 1056 X010
// MATCHING 801264b4 1056
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
typedef struct { void **p; int x, y; } AT3;

extern unsigned char D_8009C93F, D_8009C942;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern short D_800A60EA;
extern AT3 D_8012F3C4[];
extern TObj *FUN_8002dcc8(int, int, B12 *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);
extern void FUN_80026e0c(int, int);
extern void FUN_80026c50(int, int, int);

void func_801264B4(TObj *o)
{
    B12 v;

    switch (o->state) {
    case 2:
        v = *(B12 *)&o->a;
        o->d90 = (int)FUN_8002dcc8(5, 0xa, &v);
        o->anim = D_8012F3C4[o->subtype].p[2];
        FUN_8001fe94(o, 0);
        o->state++;
        break;
    case 3: {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        FUN_8005a8a8(0x7f, 0, 0);
        o->timer = 300;
        o->state++;
        break;
    }
    case 4:
        if (--o->timer > 0)
            break;
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        v = *(B12 *)&o->a;
        o->d90 = (int)FUN_8002dcc8(5, 0xb, &v);
        o->anim = D_8012F3C4[o->subtype].p[2];
        FUN_8001fe94(o, 0);
        o->state = 15;
        break;
    case 7:
        v = *(B12 *)&o->a;
        o->d90 = (int)FUN_8002dcc8(5, 0xc, &v);
        o->anim = D_8012F3C4[o->subtype].p[2];
        FUN_8001fe94(o, 0);
        o->state++;
        break;
    case 8: {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        FUN_80026e0c(0x59, 1);
        FUN_80026e0c(0x5d, 1);
        FUN_80026e0c(0x5e, 1);
        FUN_80026e0c(0x5f, 1);
        FUN_80026e0c(0x73, 1);
        FUN_80026c50(0x60, 1, 1);
        FUN_8005a9a4(0x7f, 1);
        o->timer = 300;
        o->state++;
        break;
    }
    case 9:
        if (--o->timer > 0)
            break;
        v = *(B12 *)&o->a;
        o->d90 = (int)FUN_8002dcc8(5, 0xd, &v);
        o->anim = D_8012F3C4[o->subtype].p[2];
        FUN_8001fe94(o, 0);
        o->state++;
        break;
    case 10: {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        v = *(B12 *)&o->a;
        o->d90 = (int)FUN_8002dcc8(5, 0xe, &v);
        o->anim = D_8012F3C4[o->subtype].p[2];
        FUN_8001fe94(o, 0);
        o->state = 15;
        break;
    }
    case 0:
    case 5:
    case 11:
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 1:
    case 6:
    case 12:
        o->state++;
        break;
    case 13:
        v = *(B12 *)&o->a;
        o->d90 = (int)FUN_8002dcc8(5, 0xe, &v);
        o->anim = D_8012F3C4[o->subtype].p[2];
        FUN_8001fe94(o, 0);
        o->state = 15;
        break;
    case 14:
        break;
    case 15: {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        o->anim = D_8012F3C4[o->subtype].p[0];
        FUN_8001fe94(o, 0);
        D_800A603C = 1;
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_800A60EA = 0;
        D_800A603D = 0;
        D_800A603E = 0;
        o->animFrame = 1;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
    }
}
