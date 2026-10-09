// FUNC 80127b78 656 X010
// MATCHING 80127b78 656
#include "TOBJ.H"

extern unsigned char D_8009C93F, D_8009C942;
extern unsigned char D_8009C940[];
extern unsigned char D_8009C941;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern short D_800A6066;
extern unsigned char D_8009D078;
extern short D_1F80016A;
extern short D_800A604A;
extern int FUN_8002dcc8(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern void FUN_8001f8e4(TObj *);
extern void FUN_80026e0c(int, int);

#define ANIMS(o) (*(void ***)((char *)(o) + 0xa8))

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void func_80127B78(TObj *o)
{
    short t = o->state;
    unsigned char *k;
    TObj *p;

    switch ((unsigned char)t) {
    case 0:
        o->b68 = 0;
        o->state++;
        setAnim(o, ANIMS(o)[4]);
        if (D_8009D078 >= 2) o->step = 2;
        break;
    case 1:
        if (o->b68) {
            o->state = ++t;
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            o->d90 = FUN_8002dcc8(2, 0x14, &o->a);
            o->animFrame = o->b68 & 1;
            setAnim(o, ANIMS(o)[5]);
            break;
        }
        k = D_8009C940;
        if (*k == 0) break;
        if (D_8009C941 == 3) {
            *k = 0;
            o->state++;
            D_8009C93F = 1;
            D_8009C942 = 1;
            FUN_8001f8e4(o);
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            D_800A6066 = 1 - o->animFrame;
            o->d90 = FUN_8002dcc8(2, 0x16, &o->a);
            setAnim(o, ANIMS(o)[5]);
        } else if (D_8009C941 == 0x8b) {
            *k = 0;
            FUN_80026e0c(0x8b, 1);
            D_8009C93F = 1;
            D_8009C942 = 1;
            o->step = 2;
            o->state = 3;
            D_8009D078 = 2;
        }
        break;
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
    }
    if (D_1F80016A >= 0xc8b) D_800A604A = 0xc8a;
}
