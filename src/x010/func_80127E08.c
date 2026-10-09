// FUNC 80127e08 756 X010
// MATCHING 80127e08 756
#include "TOBJ.H"

extern unsigned char D_8009C93F, D_8009C942;
extern unsigned char D_8009C940[];
extern unsigned char D_8009C941;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern short D_800A6066;
extern unsigned char D_8009D078;
extern int FUN_8002dcc8(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern void FUN_8001f8e4(TObj *);
extern void FUN_8005a9a4(int, int);

#define ANIMS(o) (*(void ***)((char *)(o) + 0xa8))

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void func_80127E08(TObj *o)
{
    unsigned char *k;
    TObj *p;
    int m, n;

    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->state++;
        setAnim(o, ANIMS(o)[4]);
        break;
    case 1:
        if (o->b68) {
            o->state++;
            D_8009C93F = 1;
            D_8009C942 = 1;
            FUN_8001f8e4(o);
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            D_800A6066 = 1 - o->animFrame;
            o->d90 = FUN_8002dcc8(2, 0x18, &o->a);
            setAnim(o, ANIMS(o)[5]);
            D_8009D078 = 3;
            break;
        }
        k = D_8009C940;
        if (*k == 0) break;
        if (D_8009C941 != 3) break;
        *k = 0;
        o->state++;
        D_8009C93F = 1;
        D_8009C942 = 1;
        FUN_8001f8e4(o);
        m = 2;
        n = 0x16;
        goto talk;
    case 2:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        o->state = 0;
        break;
    case 3:
        o->state++;
        FUN_8001f8e4(o);
        m = 2;
        n = 0x17;
    talk:
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        D_800A6066 = 1 - o->animFrame;
        o->d90 = FUN_8002dcc8(m, n, &o->a);
        setAnim(o, ANIMS(o)[5]);
        break;
    case 4:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->state++;
        break;
    case 5:
        FUN_8005a9a4(0x40, 0);
        o->timer = 0x168;
        o->state++;
        break;
    case 6:
        if (--o->timer == -1) {
            o->d90 = FUN_8002dcc8(2, 0x18, &o->a);
            o->anim = ANIMS(o)[5];
            FUN_8001fe6c(o);
            D_8009D078 = 3;
            o->state = 2;
        }
        break;
    }
}
