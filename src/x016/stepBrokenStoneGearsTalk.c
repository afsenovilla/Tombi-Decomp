// FUNC 80117870 296 X016
// MATCHING 80117870 296
// VERSION SLPS_01144 x16 80116C20 296
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x016 80117870 (x016/func_80117870.c): 8192 -> 32, 16 -> 4096.
#include "spad.h"
#include "gcd.h"
#include "gv6.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short s0, s2; } S2;

extern void func_801174B8(TObj *);
extern void func_8011767C(TObj *);

void func_80117870(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GCD.bce05 == 0xff) break;
        if ((unsigned short)((*(S2 **)&PLAYER.obj.h)->s2 - 0x82) >= 0x28) break;
        if (PLAYER.obj.y.p.whole != -0x3d) break;
        if (GVARS.b40 != 0 && GVARS.b41 == 0x3e) {
            o->step = 2;
            o->state = 0;
        } else if ((*(volatile unsigned short *)&GV6.s70 & 0x1000) && (SPAD.s1fc & 0x20)) {
            GVARS.b3e = 1;
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        func_801174B8(o);
        break;
    case 2:
        func_8011767C(o);
        break;
    }
}
#else
#include "spad.h"
#include "gcd.h"
#include "gv6.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short s0, s2; } S2;

extern void brokenStoneGearsTalk(TObj *);
extern void brokenStoneGearsRepair(TObj *);

void stepBrokenStoneGearsTalk(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GCD.bce05 == 0xff) break;
        if ((unsigned short)((*(S2 **)&PLAYER.obj.h)->s2 - 0x82) >= 0x28) break;
        if (PLAYER.obj.y.p.whole != -0x3d) break;
        if (GVARS.b40 != 0 && GVARS.b41 == 0x3e) {
            o->step = 2;
            o->state = 0;
        } else if ((*(volatile unsigned short *)&GV6.s70 & 0x10) && (SPAD.s1fc & 0x2000)) {
            GVARS.b3e = 1;
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        brokenStoneGearsTalk(o);
        break;
    case 2:
        brokenStoneGearsRepair(o);
        break;
    }
}
#endif
