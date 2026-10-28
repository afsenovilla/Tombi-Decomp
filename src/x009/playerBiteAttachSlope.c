// FUNC 8011d378 116 X009
// MATCHING 8011d378 116
#include "TOBJ.H"
#include "raw7.h"

extern TObj *BITTEN_OBJECT;
extern void playerSetAngleFromSlope(TObj *);

void func_8011D378(TObj *o)
{
    o->b69 = 0;
    (*(unsigned char*)&o->wac) = 3;
    o->b9c = 0;
    o->velX = 0;
    o->velY = 0;
    o->velH = 0;
    o->velV = 0;
    o->wb2 = 0;
    playerSetAngleFromSlope(o);
    BITTEN_OBJECT->d8c = (0x100 - o->d8c) & 0xff;
    o->state = 3;
}
