// FUNC 80125ae4 164 X001
// MATCHING 80125ae4 164
#include "TOBJ.H"
extern short D_1F80019E;
extern int func_80044424(TObj *, TObj *);
extern int func_800428C0(TObj *, TObj *);

void func_80125AE4(TObj *a, TObj *b)
{
    int r;
    short s;

    if (b->subtype == 0) {
        r = func_80044424(a, b);
        s = r;
        if (s >= 0) {
            b->w9a = (short)func_800428C0(a, b) >> 1;
            if (s == 2) b->animFrame = r * 2;
            else b->animFrame = (a->animFrame & 1) * 2;
            D_1F80019E = 0;
        }
    }
}
