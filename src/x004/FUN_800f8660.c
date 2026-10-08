// FUNC 800f8660 136 X004
// MATCHING 800f8660 136
#include "TOBJ.H"
extern unsigned char DAT_801152e8[];
extern void func_8001e5f4(int a, int b);
extern void func_8001fe94(TObj *o, int n);
extern char DAT_80010ca0[];

void FUN_800f8660(TObj *o)
{
    unsigned char b;
    if (o->step != 0x49) {
        *((char *)&o->da0 + 2) = 1;
    }
    *(char *)&o->wac = 0;
    func_8001e5f4(0x1c, 0x7f);
    *(signed char *)&o->b0f = -8;
    o->anim = DAT_80010ca0;
    func_8001fe94(o, 4);
    b = DAT_801152e8[o->wb0];
    o->state = o->state + 1;
    o->d8c = b;
}
