// FUNC 800ee7e0 172 X006
// MATCHING 800ee7e0 172
#include "TOBJ.H"
extern void func_8001fec0(TObj *o);
extern void setVelXFromPadLR(TObj *o);
extern void adjustVelYFromPadDown(TObj *o);
extern void func_8001fd94(TObj *o);
extern short GVARS_PLAYER_DRIFT_H;
extern short GVARS_PLAYER_DRIFT_Y;

void playerAirControlTimed(TObj *o)
{
    func_8001fec0(o);
    { int *p = &o->h->raw; *p += GVARS_PLAYER_DRIFT_H << 7; } o->y.raw += GVARS_PLAYER_DRIFT_Y << 7;
    setVelXFromPadLR(o);
    o->h->raw += o->velX << 8;
    o->timer = o->timer - 1;
    if (o->timer <= 0) {
        o->timer = 0;
        adjustVelYFromPadDown(o);
        func_8001fd94(o);
    }
}
