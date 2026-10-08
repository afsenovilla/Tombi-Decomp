// FUNC 8011fe38 144 X003
// MATCHING 8011fe38 144
#include "TOBJ.H"
extern short func_8004b57c(void);
extern short DAT_1f8003bc;
extern short DAT_1f80019e;
extern TObj *DAT_1f8003c0;

void FUN_8011fe38(TObj *a, TObj *b)
{
    if (b->d94 == 0 && func_8004b57c() != 0) {
        a->b9e = 4;
        a->wba = -3;
        a->velY = 0;
        DAT_1f80019e = 0;
        a->wb8 = DAT_1f8003bc;
        b->b68 = 2;
        DAT_1f8003c0 = b;
        b->animFrame = a->animFrame & 1;
    }
}
