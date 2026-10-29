// FUNC 8011af20 184 X010
// MATCHING 8011af20 184
// VERSION SLPS_01144 x10 80119AD4 184
#if defined(VERSION_SLPS_01144)
#include "gv2.h"
#include "TOBJ.H"
extern void cullObjectToDrawList(TObj *);
extern unsigned char D_8009EE6B; /* GV2.bc3 */
extern void FUN_80018838(TObj *);

void updateSceneryModel(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009EE6B & 0x40) o->y.p.whole += 0xa0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b04++;
        break;
    case 1:
        cullObjectToDrawList(o);
        break;
    case 2:
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
#else
#include "gv2.h"
#include "TOBJ.H"
extern void cullObjectToDrawList(TObj *, int);
extern void FUN_80018838(TObj *);

void updateSceneryModel(TObj *o)
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
#endif
