// FUNC 800ef388 216 X000
// MATCHING 800ef388 216
#include "TOBJ.H"
extern void func_8001e5f4(int a, int b);
extern short func_8003fd78(TObj *o, int a, int b);
extern unsigned char *DAT_8009c330;
extern unsigned char *DAT_8009f0ec;

void FUN_800ef388(TObj *o)
{
    if (o->b69 != 0) {
        func_8001e5f4(0x1c, 0x7f);
        DAT_8009c330[8] = 0;
        o->ba7 = 0;
        *(char *)&o->wac = 0;
        o->b9c = 0;
        o->b9e = 0;
        *(char *)&o->waa = 0;
        o->step = 0;
        o->state = 0;
    }
    if (DAT_8009f0ec != 0 && DAT_8009f0ec[2] == 0x14 && func_8003fd78(o, 0, 0) != 0) {
        func_8001e5f4(0x1c, 0x7f);
        DAT_8009c330[8] = 0;
        o->ba7 = 0;
        *(char *)&o->wac = 0;
        o->b9c = 0;
        o->b9e = 0;
        *(char *)&o->waa = 0;
        o->step = 0;
        o->state = 0;
    }
}
