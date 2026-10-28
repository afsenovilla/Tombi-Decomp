// FUNC 800f35e0 188 X016
// MATCHING 800f35e0 188
#include "TOBJ.H"
extern char *PLAYER_EXT;
extern unsigned char SLOPE_TILT_TABLE[];
extern void FUN_8001e5f4(int, int);
extern void FUN_8001e4f0(int);
extern void spawnGroundDebris(int, short, int, int);

void playerStepEntryLandDebris(TObj *o)
{
    FUN_8001e5f4(0x1c, 0x7f);
    PLAYER_EXT[8] = 0;
    o->step = 3;
    o->state = 1;
    o->ba7 = 0;
    *(char *)&o->wac = 0;
    o->b9c = 0;
    o->d8c = SLOPE_TILT_TABLE[o->wb0];
    if (o->bbe & 0x20) {
        FUN_8001e4f0(0x3e);
        spawnGroundDebris(o->h->p.whole, o->y.p.whole, o->d->p.whole, 0);
        spawnGroundDebris(o->h->p.whole, o->y.p.whole, o->d->p.whole, 0);
    }
}
