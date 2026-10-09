// FUNC 801175a0 368 X014
// MATCHING 801175a0 368
#include "TOBJ.H"

extern int D_1F8002D4[];
extern void *D_80129F98;
extern unsigned char D_8009C975;
extern unsigned char D_8009C93C;
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void AnimAdvance(TObj *);
extern void ObjFree(TObj *);
extern void FUN_80059d44(int);

void func_801175A0(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 0x10;
        o->b0d = 0;
        o->d3c = D_1F8002D4[0];
        o->anim = D_80129F98;
        AnimLoadDuration(o);
        break;
    case 1:
        if (ObjCullRegister(o)) AnimAdvance(o);
        if (o->subtype != 1) break;
        switch (o->step) {
        case 0:
            if (D_8009C975 != 0) break;
            D_8009C93C = 2;
            o->animFrame = 0x60;
            FUN_80059d44(0x60);
            o->step++;
            break;
        case 1:
            FUN_80059d44((short)o->animFrame);
            if (D_8009C975 == 3) o->step--;
            break;
        case 2:
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
