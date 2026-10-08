// FUNC 80122de8 304 X014
// MATCHING 80122de8 304
#include "TOBJ.H"

extern int D_1F800334;
extern unsigned short D_8009C962;
extern void func_8012284C(TObj *);
extern void func_80122AAC(TObj *);
extern void FUN_80018790(TObj *);

void func_80122DE8(TObj *o)
{
    switch (o->b04) {
    case 0: {
        int *pp = &D_1F800334;
        int *q = (int *)*pp;
        q += 1;
        if (D_8009C962 == 7) o->da0 = *pp + q[5];
        else o->da0 = *pp + q[0];
        o->ba4 = 1;
        o->box0 = 0x10;
        o->box1 = 0x20;
        o->box2 = 0x140;
        o->box3 = 0x140;
        o->b0a = 0x11;
        o->b0f = 0;
        o->b0c = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b04++;
        break;
    }
    case 1:
        switch (o->step) {
        case 0:
            func_8012284C(o);
            break;
        case 1:
            func_80122AAC(o);
            break;
        }
        break;
    case 2:
    case 3:
        FUN_80018790(o);
        break;
    }
}
