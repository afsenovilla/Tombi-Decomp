// FUNC 8012d914 340 X003
// MATCHING 8012d914 340
#include "TOBJ.H"

extern short D_8007A3F0[];
extern short D_80135E40[];
extern void *D_8013A708;
extern int AnimAdvance(TObj *);
extern void FUN_8001f8e4(TObj *);
extern int FUN_8001f9e0(void);
extern void FUN_8001fe6c(TObj *);

void func_8012D914(TObj *o)
{
    switch (o->state) {
    case 0:
        if (--o->timer != -1)
            return;
        o->active = 3;
        o->timer = 0x78;
        o->wac = 0x1c;
        o->state++;
        break;
    case 1:
        o->ba7 += 2;
        o->y.raw += (short)(D_8007A3F0[o->ba7] << 2);
        AnimAdvance(o);
        if (--o->timer != -1)
            return;
        o->active = 1;
        o->state = 0;
        o->step++;
        FUN_8001f8e4(o);
        o->timer = D_80135E40[FUN_8001f9e0() & 0xf];
        if (o->timer == 0)
            o->wb4 = 0;
        else
            o->wb4 = 1;
        o->wac = 0x1c;
        break;
    default:
        return;
    }
    o->anim = D_8013A708;
    FUN_8001fe6c(o);
}
