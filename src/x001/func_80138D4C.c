// FUNC 80138d4c 304 X001
// MATCHING 80138d4c 304
#include "TOBJ.H"

extern int D_1F8002D4[];
extern void *D_8013E72C[];
extern void func_80138550(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);

void func_80138D4C(TObj *o)
{
    switch (o->subtype) {
    case 0:
        func_80138550(o);
        break;
    case 1:
        switch (o->b04) {
        case 0:
            o->active = 2;
            o->w1e = 7;
            *(signed char *)&o->b0f = -9;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            o->b0d = 0x80;
            o->d3c = D_1F8002D4[0];
            o->anim = D_8013E72C[0];
            AnimLoadDuration(o);
            o->b04++;
            break;
        case 1:
            ObjCullRegister(o);
            if (AnimAdvance(o) != 0) {
                o->b04++;
            }
            break;
        case 2:
            o->b04++;
            break;
        case 3:
            FUN_80018790(o);
            break;
        }
        break;
    }
}
