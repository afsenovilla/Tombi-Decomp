// FUNC 800ec378 168 X018
// MATCHING 800ec378 168
#include "TOBJ.H"
extern void func_800202b4(void);
extern void weaponTrailFollowStep(TObj *o);
extern void weaponTrailOrbitStep(TObj *o);
extern int func_8001fec0(TObj *o);
extern unsigned char DAT_800a6039;
extern unsigned char DAT_800a6047;

void updateWeaponTrailObjectAlt(TObj *o)
{
    func_800202b4();
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
    case 9:
        o->visible = DAT_800a6039;
        o->b0f = DAT_800a6047 - 1;
        if (func_8001fec0(o) != 0) {
            o->b04 = 3;
        }
        break;
    }
}
