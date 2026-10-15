// FUNC 80126d04 336 X004
// MATCHING 80126d04 336
#include "spad.h"
#include "TOBJ.H"
typedef struct { char pad[0xc4]; unsigned short wc4, wc6, wc8; } X4;

extern void blackCrowPerchIdle(TObj *);
extern void blackCrowPerchLook(TObj *);
extern void blackCrowPerchCaw(TObj *);
extern void blackCrowFlyAround(TObj *);
extern void blackCrowWatchPlayer(TObj *);
extern short probeFloorPlaneAt(TObj *, short, short);

static __inline__ void approach(TObj *o, unsigned short t)
{
    short d = o->d->p.whole;
    int s;

    if (t != d) {
        s = (t - d) >> 4;
        if ((unsigned)(s + 1) < 3) {
            o->d->p.whole = t;
        } else {
            o->d->p.whole = d + s;
        }
    }
}

void blackCrowPerched(TObj *o)
{
    switch (o->state) {
    case 0:
        blackCrowPerchIdle(o);
        goto common;
    case 1:
        blackCrowPerchLook(o);
        goto common;
    case 2:
        blackCrowPerchCaw(o);
    common:
        if (probeFloorPlaneAt(o, o->h->p.whole, o->y.p.whole + 0x18) == 0) {
            o->state = 3;
            o->substep = 0;
        }
        break;
    case 3:
        blackCrowFlyAround(o);
        break;
    case 4:
        blackCrowWatchPlayer(o);
        break;
    }
    if ((*(short*)&SPAD.s1c8) == 0) {
        approach(o, ((X4 *)o)->wc8);
    } else {
        approach(o, ((X4 *)o)->wc4);
    }
}
