// VERSION BETA_980203 beta980203 80032988 980
#if defined(VERSION_BETA_980203)
// Beta version: USA C of src/spawnPlayerPowerEffect.c: PLAYER = 7 for every power (USA not for 2).
#include "plx.h"
#include "TOBJ.H"
typedef struct { int a, y, b; } P3;
extern P3 PLAYER_POS;
extern unsigned char PLAYER;
extern TObj *ObjAlloc(void);
extern void spawnPlayerPowerAura(int a, int b, int c);

static __inline__ void spawn(short sub, int idx)
{
    TObj *o = ObjAlloc();
    if (o != 0) {
        o->active = 1;
        o->type = 0x4e;
        o->subtype = sub;
        o->b0c = idx;
        o->a.raw = PLAYER_POS.a;
        o->y.raw = PLAYER_POS.y;
        o->b.raw = PLAYER_POS.b;
        o->step = 0;
    }
}

void spawnPlayerPowerEffect(short n)
{
    PLAYER = 7;
    PLX.b6104 = 2;
    switch (n) {
    case 0:
        spawnPlayerPowerAura(0, 0, 0);
        spawn(n, 1);
        break;
    case 1:
        spawnPlayerPowerAura(1, 0, 0);
        spawn(n, 5);
        spawn(n, 6);
        spawn(n, 7);
        spawn(n, 8);
        spawn(n, 9);
        spawn(n, 10);
        break;
    case 2:
        spawn(n, 0);
        spawn(n, 1);
        spawn(n, 2);
        break;
    }
}
#endif
