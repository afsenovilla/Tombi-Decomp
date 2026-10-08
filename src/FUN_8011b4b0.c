// FUNC 8011b4b0 300 X000
// MATCHING 8011b4b0 300
#include "TOBJ.H"
extern unsigned char DAT_a, DAT_b, DAT_c, DAT_d, DAT_e;
extern TObj *FUN_8002dc50(int, int, int, int);
extern void FUN_80018980(TObj *);

void FUN_8011b4b0(TObj *o)
{
    TObj *q;
    switch (o->step) {
    case 0:
        if (o->visible != 0)
            o->step++;
        else
            o->step = 2;
        break;
    case 1:
        if (DAT_a == 0) {
            DAT_b = 1;
            DAT_c = 1;
            DAT_d = 0;
            o->step++;
        }
        break;
    case 2:
        o->step++;
        *(TObj **)&o->category = FUN_8002dc50(10, 0, 100, 236);
        break;
    case 3:
        q = *(TObj **)&o->category;
        if (q->b04 == 2) {
            q->b04 = 3;
            DAT_b = 0;
            DAT_e = 0;
            DAT_c = 0;
            FUN_80018980(o);
        }
        break;
    }
}
