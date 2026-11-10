// VERSION BETA_980203 x00 800F184C 424
#if defined(VERSION_BETA_980203)
// Beta version: written from the disc (PAL updateWeaponTrailObject.c in an earlier form): width 0xA3 or 0x33 from func_8005D190, byte 0x6B = 0x40, subtypes 0-2 follow, 3-4 orbit.
#include "TOBJ.H"
typedef struct { short k; short w1e; void **anims; int pad2; } T12;
extern T12 D_8011846C[];
extern int SPR_DATA[];
extern int func_8005D190(void);
extern void AnimLoadDuration(TObj *);
extern void ObjCullRegister(TObj *);
extern void weaponTrailFollowStep(TObj *);
extern void weaponTrailOrbitStep(TObj *);
extern void ObjFree(TObj *);

void updateWeaponTrailObject(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->w1e = (func_8005D190() == 1 || func_8005D190() == 2) ? 0xa3 : 0x33;
        o->d3c = SPR_DATA[D_8011846C[o->subtype].k];
        o->anim = D_8011846C[o->subtype].anims[o->b0c];
        AnimLoadDuration(o);
        o->b0a = 13;
        o->b0d = 0x80;
        o->b6b = 0x40;
        *(signed char *)&o->b0f = -7;
        o->d8c = 0;
        o->category |= 0x80;
        o->b04++;
        break;
    case 1:
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
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
#endif
