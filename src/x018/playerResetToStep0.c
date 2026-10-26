// FUNC 801049c0 68 X018
// MATCHING 801049c0 68
#include "TOBJ.H"
typedef struct { char p[8]; unsigned char b; } G;
extern G *PLAYER_EXT;
extern unsigned char SLOPE_TILT_TABLE[];
void playerResetToStep0(TObj *o)
{
    PLAYER_EXT->b = 0;
    o->b9c = 0;
    o->ba7 = 0;
    o->wb2 = 0;
    o->velY = 0;
    *(unsigned char *)&o->wac = 0;
    o->d8c = SLOPE_TILT_TABLE[o->wb0];
    o->step = 0;
    o->state = 0;
}
