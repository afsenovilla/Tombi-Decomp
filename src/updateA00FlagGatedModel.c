// FUNC 8011f1d0 184 X000
// MATCHING 8011f1d0 184
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"
extern void ObjListPush_1F800224(void);
extern void ObjFreeDup(void);

void updateA00FlagGatedModel(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->active = 2;
        if (GCD.bcdac == 0xff) {
            int *pp = &SPAD.i334;
            pp = (int *)(*pp + ((int *)*(int *volatile *)pp)[12]);
            o->da0 = (int)pp;
        }
        break;
    case 1:
        o->visible = 1;
        ObjListPush_1F800224();
        break;
    case 2:
    case 3:
        ObjFreeDup();
        break;
    }
}
