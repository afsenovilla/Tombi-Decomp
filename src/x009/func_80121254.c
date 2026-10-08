// FUNC 80121254 112 X009
// MATCHING 80121254 112
#include "TOBJ.H"
extern short func_8004b57c(void);
extern short DAT_1f8003bc;
extern short DAT_1f80019e;
extern TObj *DAT_1f8003c0;

void func_80121254(TObj *a, TObj *b)
{
    if (func_8004b57c() != 0) {
        a->b9e = 4;
        a->wba = 0;
        a->velY = 0;
        DAT_1f80019e = 0;
        a->wb8 = DAT_1f8003bc;
        b->ba7 = 1;
        DAT_1f8003c0 = b;
    }
}
