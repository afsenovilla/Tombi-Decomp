// VERSION BETA_980203 x00 800F17EC 96
#if defined(VERSION_BETA_980203)
// Beta version: PAL x008 updateWeaponTrailObjectAlt.c without its case 9 (subtypes 0-2 follow, 3-4 orbit).
#include "TOBJ.H"
extern int ObjCullRegister(TObj *);
extern void weaponTrailFollowStep(TObj *);
extern void weaponTrailOrbitStep(TObj *);

void updateWeaponTrailObjectAlt(TObj *o)
{
    ObjCullRegister(o);
    switch (o->subtype) {
    case 0:
    case 1:
    case 2:
        weaponTrailFollowStep(o);
        break;
    case 3:
    case 4:
        weaponTrailOrbitStep(o);
        break;
    }
}
#endif
