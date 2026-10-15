// FUNC 80118f7c 564 X009
// MATCHING 80118f7c 564
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"

typedef struct { char p[0xb4]; short s[15]; } XS;
#define S(o, off) (((XS *)(o))->s[((off) - 0xb4) / 2])

extern void *D_8012E9E8[], *D_8012E9BC, *D_8012E9C8;
extern void AnimLoadDuration(TObj *o);
extern int AnimAdvance(TObj *o);
extern int ObjCullRegister(TObj *o);
extern void rotatingPlaneSpriteSetQuad(TObj *o);
extern void rotatingPlaneSpriteSpawnPetals(TObj *o);
extern void removeItemFromInventory(int, int);

void rotatingPlaneSpriteBStep(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GCD.bd07e) {
            o->timer = 0x10;
            o->step++;
        }
    chk:
        if (o->subtype == 1) return;
        ObjCullRegister(o);
        break;
    case 1:
        if (--o->timer == -1) o->step++;
        goto chk;
    case 2:
        if (o->subtype == 0) {
            S(o, 0xb4) = -0x35;
            S(o, 0xb6) = -0x2c;
            S(o, 0xb8) = 0;
            S(o, 0xbc) = 0x34;
            S(o, 0xbe) = -0x2c;
            S(o, 0xc0) = 0;
            S(o, 0xc4) = -0x35;
            S(o, 0xc6) = 0x14;
            S(o, 0xc8) = 0;
            S(o, 0xcc) = 0x34;
            S(o, 0xce) = 0x14;
            S(o, 0xd0) = 0;
            o->anim = D_8012E9E8[0];
        }
        o->timer = 8;
        o->step++;
        ObjCullRegister(o);
        break;
    case 3:
        if (--o->timer == -1) o->step++;
        ObjCullRegister(o);
        break;
    case 4:
        o->w1e = 1;
        o->step++;
        if (o->subtype == 0) {
            o->subtype = 0;
            o->b0c = 0;
            o->anim = D_8012E9BC;
            rotatingPlaneSpriteSetQuad(o);
        } else {
            o->subtype = 1;
            o->b0c = 3;
            o->anim = D_8012E9C8;
            AnimLoadDuration(o);
            rotatingPlaneSpriteSetQuad(o);
            GVARS.b40 = 0;
            GVARS.b3f = 0;
            GVARS.b42 = 0;
            GVARS.b3e = 0;
            removeItemFromInventory(0x8a, 1);
        }
        ObjCullRegister(o);
        break;
    case 5:
        if (o->subtype == 1) {
            AnimAdvance(o);
            rotatingPlaneSpriteSpawnPetals(o);
        }
        ObjCullRegister(o);
        break;
    }
}
