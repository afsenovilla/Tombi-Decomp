// FUNC 80125868 124 X001
// MATCHING 80125868 124
#include "TOBJ.H"

extern int func_801253AC(TObj *, TObj *, int, int);

void func_80125868(TObj *a, TObj *b)
{
    if (b->subtype == 0) {
        func_801253AC(a, b, 0, b->d8c);
    } else if (!func_801253AC(a, b, b->subtype, b->d8c)) {
        func_801253AC(a, b, 2, (b->d8c + 0x400) & 0xfff);
    }
}
