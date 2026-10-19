// FUNC 80111880 156 X014
// MATCHING 80111880 156
#include "TOBJ.H"
extern void playerPowerAuraFacingStep(TObj *);
extern void playerPowerAuraHiddenStep(TObj *);

void playerPowerAuraStep(TObj *o)
{
    switch (o->subtype) {
    case 0:
        playerPowerAuraFacingStep(o);
        break;
    case 1:
        playerPowerAuraHiddenStep(o);
        break;
    }
    if (o->w22 != 0) {
        o->w22--;
        if (o->w22 <= 0)
            o->active = 1;
    }
    if (o->b6a != 0) {
        o->b6a = 0;
        o->w22 = 3;
    }
}
