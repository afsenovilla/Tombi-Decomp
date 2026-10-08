// FUNC 801251c4 168 X003
// MATCHING 801251c4 168
#include "TOBJ.H"
extern int D_1F8002D4[];
extern void AnimLoadDuration(TObj *);
extern int func_800202B4(TObj *);
extern void FUN_80018790(TObj *);

void func_801251C4(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b0d = 0;
        o->w1e = 8;
        o->box0 = 8;
        o->box1 = 0x10;
        o->box2 = 8;
        o->box3 = 0x10;
        o->d3c = D_1F8002D4[0];
        AnimLoadDuration(o);
        break;
    case 1:
        func_800202B4(o);
        break;
    case 2:
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
