// FUNC 800f434c 132 X001
// MATCHING 800f434c 132
#include "TOBJ.H"
extern unsigned char DAT_801152e8[];
extern void func_8001e5f4(int a, int b);

void FUN_800f434c(TObj *o)
{
    func_8001e5f4(0x1c, 0x7f);
    o->b9c = 0;
    o->ba7 = 0;
    *(char *)&o->wac = 0;
    o->b9d = 0;
    o->d8c = DAT_801152e8[o->wb0];
    if (o->bbe & 0x20) {
        o->step = 0x1f;
        o->state = 1;
    } else {
        o->step = 1;
        o->state = 0;
    }
}
