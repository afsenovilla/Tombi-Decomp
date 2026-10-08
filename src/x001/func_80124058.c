// FUNC 80124058 80 X001
// MATCHING 80124058 80
#include "TOBJ.H"
extern short FUN_80042fbc(TObj *, TObj *);

void func_80124058(TObj *a, TObj *b)
{
    b->b69 = 0;
    if (FUN_80042fbc(a, b)) {
        *(unsigned char *)&a->da0 = 0x35;
        b->b69 = 1;
    }
}
