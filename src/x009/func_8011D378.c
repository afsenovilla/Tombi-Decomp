// FUNC 8011d378 116 X009
// MATCHING 8011d378 116
#include "TOBJ.H"
#include "raw7.h"

extern TObj *D_8009D2E8;
extern void func_800EEA3C(TObj *);

void func_8011D378(TObj *o)
{
    o->b69 = 0;
    U8(o, 0xac) = 3;
    o->b9c = 0;
    o->velX = 0;
    o->velY = 0;
    o->velH = 0;
    o->velV = 0;
    o->wb2 = 0;
    func_800EEA3C(o);
    D_8009D2E8->d8c = (0x100 - o->d8c) & 0xff;
    o->state = 3;
}
