// FUNC 8012767c 132 X001
// MATCHING 8012767c 132
#include "TOBJ.H"

extern int func_80127074(TObj *, TObj *, int, int);

int func_8012767C(TObj *a, TObj *b)
{
    if (b->subtype == 0) return func_80127074(a, b, 0, b->d8c);
    if (func_80127074(a, b, b->subtype, b->d8c)) return 1;
    return func_80127074(a, b, 2, (b->d8c + 0x400) & 0xfff);
}
