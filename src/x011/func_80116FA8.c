// FUNC 80116fa8 228 X011
// MATCHING 80116fa8 228
#include "TOBJ.H"

extern void *D_8011C420;
extern int D_1F8002F0[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void func_80116E54(TObj *);
extern void FUN_80018790(TObj *);
static __inline__ void setanim(TObj *p)
{
    p->d3c = D_1F8002F0[0];
    p->wac = 0;
    p->anim = D_8011C420;
    AnimLoadDuration(p);
}
static __inline__ void setbox(TObj *p, short a, short b, short c, short d)
{
    p->box0 = a;
    p->box1 = b;
    p->box2 = c;
    p->box3 = d;
}

void func_80116FA8(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        setbox(o, 0x10, 0x20, 0x10, 0x20);
        o->b0d = 1;
        o->w08 = 0x7952;
        o->w1e = 9;
        setanim(o);
        break;
    case 1:
        ObjCullRegister(o);
        func_80116E54(o);
        break;
    case 2:
    case 3:
        FUN_80018790(o);
        break;
    }
}
