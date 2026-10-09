// FUNC 801280fc 1120 X010
// MATCHING 801280fc 1120
#include "TOBJ.H"
#include "raw7.h"

typedef struct { char p[0x4c]; short w4c, w4e; } SC;

extern TObj D_800A6038;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned char D_8009C942, D_8009D078, D_8009CF1C, D_8009C975, D_8009C93C;
extern short D_8009CD94, D_8009CD96, D_8009CDA0;
extern int D_8009C984[];
extern short D_1F8000F2, D_1F80016E;
extern SC *D_1F8001D4;
extern short D_8012F3C8[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_800eea7c(TObj *, int, int);
extern int FUN_8002dcc8(int, int, void *);
extern void FUN_8005a9a4(int, int);
extern void func_801271D8(TObj *, int);

#define ANIMS(o) (*(void ***)((char *)(o) + 0xa8))

void func_801280FC(TObj *o)
{
    TObj *p;
    int m, n;

    switch (o->state) {
    case 0:
        o->anim = ANIMS(o)[0];
        AnimLoadDuration(o);
        o->state++;
        D_1F8000F2 = -300;
    case 1:
        if (D_1F80016E < -0xe9) break;
        D_8009C942 = 1;
        D_800A6038.active = 5;
        D_800A603C = 5;
        D_800A603D = 0x64;
        D_800A603E = 0;
        FUN_800eea7c(&D_800A6038, 0x1e, 0);
        D_800A6038.y.p.whole = -0xd2;
        o->d34 = 0xff2e0000;
        U8(o, 0xa7) = 0;
        o->timer = 0x3c;
        o->state++;
        D_8009D078 = 4;
        break;
    case 2:
        func_801271D8(o, 1);
        if (--o->timer == -1) o->state++;
        break;
    case 3:
        func_801271D8(o, 1);
        o->anim = ANIMS(o)[5];
        AnimLoadDuration(o);
        m = 2;
        n = 0x19;
        goto talk;
    case 4:
        func_801271D8(o, 1);
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 5:
        func_801271D8(o, 1);
        FUN_800eea7c(&D_800A6038, 0x47, 0);
        U8(o, 0xa7) = 0;
        D_800A6038.y.p.whole = -0xe2;
        D_800A6038.animFrame = 0;
        o->d34 = D_800A6038.y.raw;
        o->d30 = D_800A6038.h->p.whole;
        o->state++;
        o->d90 = FUN_8002dcc8(2, 0x1a, &o->a);
        break;
    case 6:
        func_801271D8(o, 4);
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 7:
        func_801271D8(o, 2);
        o->state++;
        o->d90 = FUN_8002dcc8(2, D_8012F3C8[D_8009CF1C], &o->a);
        break;
    case 8:
        func_801271D8(o, 2);
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 9:
        func_801271D8(o, 3);
        m = 2;
        n = 0x23;
        goto talk;
    case 10:
        func_801271D8(o, 3);
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 11:
        func_801271D8(o, 2);
        m = 2;
        n = 0x24;
    talk:
        o->state++;
        o->d90 = FUN_8002dcc8(m, n, &o->a);
        break;
    case 12:
        func_801271D8(o, 2);
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 13:
        func_801271D8(o, 3);
        D_8009C984[0] |= 8;
        FUN_8005a9a4(8, 0);
        o->timer = 0x168;
        o->state++;
        break;
    case 14:
        func_801271D8(o, 3);
        if (--o->timer == -1) {
            D_8009CD94 = 10;
            D_8009CDA0 = 3;
            D_8009CD96 = 0;
            D_8009C975 = 3;
            D_8009C93C = 0;
            o->state++;
        }
        break;
    case 15:
        func_801271D8(o, 4);
        if (D_8009C975 == 1) {
            D_1F8001D4->w4c = 7;
            D_1F8001D4->w4e = 0;
        }
        break;
    }
}
