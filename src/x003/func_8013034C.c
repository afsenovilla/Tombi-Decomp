// FUNC 8013034c 364 X003
// MATCHING 8013034c 364
#include "TOBJ.H"

extern void *D_8013A368[];
extern void *D_8013A378[];
extern unsigned char D_8009C93F[], D_8009C942[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern int FUN_8002dcc8(int, int, void *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    AnimLoadDuration(o);
}

void func_8013034C(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68 == 0) break;
        o->state++;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->d90 = FUN_8002dcc8(2, 0xe, &o->a);
        o->wac = 4;
        o->animFrame = o->b68 & 1;
        setAnim(o, D_8013A378[0]);
        break;
    case 2:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wac = 0;
        o->anim = D_8013A368[0];
        o->state = 0;
        break;
    }
}
