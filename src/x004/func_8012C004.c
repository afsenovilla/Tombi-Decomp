// FUNC 8012c004 420 X004
// MATCHING 8012c004 420
#include "TOBJ.H"

extern unsigned char D_8009C93F, D_8009C942;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned char D_8009CE24[];
extern void *D_80135748[];
extern void *D_80135750[];
extern int FUN_8002dcc8(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void func_8012C004(TObj *o)
{
    TObj *p;
    int m, n;

    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->b68 = 0;
        if (D_8009CE24[0] != 0 && D_8009CE24[0] != 0xff) {
            o->state = 3;
            break;
        }
        o->state++;
        break;
    case 1:
        if (o->b68 == 0) break;
        m = 5;
        n = 9;
        goto talk;
    case 3:
        if (o->b68 == 0) break;
        m = 5;
        n = 10;
    talk:
        o->state++;
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        o->d90 = FUN_8002dcc8(m, n, &o->a);
        o->wac = 2;
        o->animFrame = o->b68 & 1;
        setAnim(o, D_80135750[0]);
        break;
    case 2:
    case 4:
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
        o->wac = 0;
        o->anim = D_80135748[0];
        break;
    }
}
