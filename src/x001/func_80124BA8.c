// FUNC 80124ba8 120 X001
// MATCHING 80124ba8 120
#include "TOBJ.H"
extern void func_801244A0(TObj *, TObj *, int, int);

void func_80124BA8(TObj *a, TObj *b)
{
    if (b->subtype == 0) {
        func_801244A0(a, b, 0, b->d8c);
    } else {
        func_801244A0(a, b, b->subtype, b->d8c);
        func_801244A0(a, b, 2, (b->d8c + 0x400) & 0xfff);
    }
}
