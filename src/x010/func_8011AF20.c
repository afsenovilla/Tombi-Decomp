// FUNC 8011af20 184 X010
// MATCHING 8011af20 184
#include "gv2.h"
#include "TOBJ.H"
extern void cullObjectToDrawList(TObj *, int);
extern void FUN_80018838(TObj *);

void func_8011AF20(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (GV2.bc3 & 0x40) o->y.p.whole += 0xa0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b04++;
        break;
    case 1:
        cullObjectToDrawList(o, 0x80);
        break;
    case 2:
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
