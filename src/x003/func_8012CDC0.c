// FUNC 8012cdc0 544 X003
// MATCHING 8012cdc0 544
#include "TOBJ.H"
typedef struct { short v[6]; } V6;

extern unsigned char D_8009C93F, D_8009C942;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned char D_8009CF03;
extern void *D_801399BC[];
extern void *D_801399CC[];
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern void FUN_8005a8a8(int, int, int);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void func_8012CDC0(TObj *o)
{
    V6 v;
    TObj *p;

    v = *(V6 *)&o->a;
    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->wac = 9;
        o->state++;
        setAnim(o, D_801399CC[0]);
        break;
    case 1:
        AnimAdvance(o);
        if (D_8009CF03 == 3) {
            o->step++;
            o->state = 0;
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            D_8009C93F = 1;
            D_8009C942 = 1;
            break;
        }
        if (o->b68 == 0) break;
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->d90 = FUN_8002dcc8(6, 6, &v);
        o->wac = 5;
        o->state++;
        o->animFrame = o->b68 & 1;
        setAnim(o, D_801399BC[0]);
        break;
    case 2:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->b68 = 0;
        o->state = 0;
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        D_8009C93F = 0;
        D_8009C942 = 0;
        FUN_8005a8a8(0xad, 0, 1);
        break;
    }
}
