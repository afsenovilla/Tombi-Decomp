// FUNC 800fd8b0 316 I0NFO2
// MATCHING 800fd8b0 316
#include "TOBJ.H"
extern void npcSet0FCheckTriggerSpots(TObj *o);
extern unsigned char GVARS_USE_ITEM_PENDING, GVARS_USE_ITEM_ID, D_8009CDAB, D_8009CE0D[], D_8009CF9E, D_8009CFA6[], D_8009CFAB, D_800A60A3;

void npcSet0FPickStep(TObj *o)
{
    o->state = 0;
    if (D_800A60A3 == 4 && D_8009CDAB == 0) {
        o->step++;
    } else if (D_8009CF9E == 1) {
        o->step = 2;
    } else {
        npcSet0FCheckTriggerSpots(o);
    }
    if (GVARS_USE_ITEM_PENDING == 1) {
        if (D_800A60A3 == 5 && GVARS_USE_ITEM_ID == 0x3D) o->step = 4;
    } else if (D_8009CE0D[0] == 2) {
        D_8009CE0D[0]++;
        o->step = 5;
    }
    if (D_8009CFA6[0] == 1) {
        D_8009CFA6[0]++;
        o->step = 7;
    } else if (D_8009CFAB == 1) {
        D_8009CFAB = 0;
        o->step = 6;
    }
}
