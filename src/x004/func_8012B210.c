// FUNC 8012b210 376 X004
// MATCHING 8012b210 376
#include "TOBJ.H"
extern int D_1F8002D4[];
extern void *D_80134CE0;
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void FUN_80018790(TObj *);
extern void func_8012AA10(TObj *);

void func_8012B210(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->box0 = 8;
        o->box1 = 0x10;
        o->box3 = 0x10;
        o->box2 = 8;
        o->w1e = 6;
        o->b0d = 0;
        o->d3c = D_1F8002D4[0];
        o->anim = D_80134CE0;
        o->category |= 0x80;
        AnimLoadDuration(o);
        o->b04 = 2;
        o->step = 3;
        o->state = 0;
        break;
    case 1:
        if (ObjCullRegister(o)) {
            if (o->step == 0) AnimAdvance(o);
        }
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            o->step = 1;
            break;
        case 1:
            AnimAdvance(o);
            break;
        case 2:
            o->b04 = 3;
            break;
        case 3:
            func_8012AA10(o);
            break;
        case 4:
            if (o->visible == 0) o->b04 = 3;
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
