// FUNC 801191b0 412 X009
// MATCHING 801191b0 412
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

extern void initRotatingPlaneSpriteB(TObj *);
extern void initRotatingPlaneSprite(TObj *);
extern void followParentRotation(TObj *);
extern void rotatingPlaneSpriteBStep(TObj *);
extern void rotatingPlaneSpriteStep(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_800187e4(TObj *);

void updateRotatingPlaneSprite(TObj *o)
{
    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            o->step++;
            break;
        case 1:
            o->b04++;
            o->step = 0;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            *(int *)&o->w5c = o->a.p.whole;
            o->d60 = o->y.p.whole;
            o->d64 = o->b.p.whole;
            if (o->animFrame & 0x80)
                initRotatingPlaneSpriteB(o);
            else
                initRotatingPlaneSprite(o);
            break;
        }
        break;
    case 1:
        if (GAME.selectedPlane != 0x20 && GVARS.playerLanded == 1) {
            o->b04++;
            break;
        }
        followParentRotation(o);
        if (o->animFrame & 0x80) {
            rotatingPlaneSpriteBStep(o);
        } else {
            FUN_800202b4(o);
            if (o->subtype != 0)
                rotatingPlaneSpriteStep(o);
        }
        break;
    case 2:
        if (GAME.selectedPlane == 0x20)
            o->b04--;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
