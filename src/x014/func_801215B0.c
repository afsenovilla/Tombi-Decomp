// FUNC 801215b0 444 X014
// MATCHING 801215b0 444
#include "TOBJ.H"

extern int D_1F800334;
extern unsigned short D_8009C962;
extern unsigned char D_8009C942;
extern int FUN_800202b4(TObj *);
extern void func_801211F0(TObj *);
extern void func_801213F4(TObj *);
extern void FUN_80018790(TObj *);

void func_801215B0(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (o->subtype == 0) {
            int *pp = &D_1F800334;
            int *q = (int *)*pp;
            q += 1;
            if (D_8009C962 == 7) o->da0 = *pp + q[2];
            else o->da0 = *pp + q[1];
            o->ba4 = 1;
            o->box0 = 0x1c;
            o->box1 = 0x38;
            o->box2 = 0x1c;
            o->box3 = 0x38;
        } else {
            int *pp = &D_1F800334;
            int *q = (int *)*pp;
            q += 1;
            if (D_8009C962 == 7) o->da0 = *pp + q[1];
            else o->da0 = *pp + q[0];
            o->ba4 = 1;
            o->box0 = 6;
            o->box1 = 0xc;
            o->box2 = 6;
            o->box3 = 0xc;
        }
        o->b0a = 0x11;
        o->b0f = 0;
        o->b0c = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b04++;
        break;
    case 1:
        if (D_8009C942 == 1) {
            FUN_800202b4(o);
        } else if (o->subtype == 0) {
            FUN_800202b4(o);
            func_801211F0(o);
        } else {
            func_801213F4(o);
        }
        break;
    case 2:
    case 3:
        FUN_80018790(o);
        break;
    }
}
