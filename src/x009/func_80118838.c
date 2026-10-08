// FUNC 80118838 216 X009
// MATCHING 80118838 216
#include "TOBJ.H"
extern int D_1F8002D4[];
extern void *D_8012E020[];
extern void AnimLoadDuration(TObj *);
extern int FUN_800202b4(TObj *);
extern int FUN_8001fec0(TObj *);
extern void ObjFree(TObj *);

static __inline__ void setanim(TObj *q, int d, void *a)
{
    q->d3c = d;
    q->anim = a;
    AnimLoadDuration(q);
}

void func_80118838(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->step = 0;
        o->b0d = 0;
        o->b0a = 0;
        o->animFrame = 0;
        o->w1e = 9;
        setanim(o, D_1F8002D4[0], D_8012E020[0]);
        break;
    case 1:
        FUN_800202b4(o);
        if (FUN_8001fec0(o)) o->b04 = 3;
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
