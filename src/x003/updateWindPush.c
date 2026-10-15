// FUNC 80130990 1196 X003
// MATCHING 80130990 1196
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
extern TObj PLAYER;
extern unsigned short D_80135EC0[], D_80135EC8[];
extern int FUN_8001f9e0(void);
extern void FUN_80018980(TObj *);

#define W1C(o) (*(short *)((char *)(o) + 0x1c))
#define U8(o, n) (*(unsigned char *)((char *)(o) + (n)))

void updateWindPush(TObj *o)
{
    TObj *pl = &PLAYER;
    short v;

    switch (o->b04) {
    case 0:
        if (GAME.purifiedAreas & 2)
            o->b04 = 2;
        else if (GAME.selectedSection < 2)
            o->b04++;
        else
            o->b04 = 3;
    clr:
        GVARS.s44 = 0;
        break;
    case 1:
        if (SPAD.b1a4) goto clr;
        switch (o->step) {
        case 0:
            if (GVARS.b3a) {
                o->step++;
                GVARS.s44 = 0x100;
            }
            break;
        case 1:
            if (PLAYER.y.p.whole >= -0x153) goto clr;
            if (U8(&PLAYER, 0xa2) | U8(&PLAYER, 0xa3)) goto clr;
            if (GVARS.b3f) goto clr;
            if (PLAYER.step == 0x34) goto clr;
            switch (PLAYER.b9c) {
            case 0:
                GVARS.s44 = 0x100;
                break;
            case 1:
                GVARS.s44 += 8;
                if (GVARS.s44 > 0x200) GVARS.s44 = 0x200;
                break;
            case 2:
                GVARS.s44 += 0x10;
                if (GVARS.s44 > 0x300) GVARS.s44 = 0x300;
                break;
            }
            break;
        }
        break;
    case 2:
        switch (o->step) {
        case 0:
            if (GAME.selectedSection < 4) {
                o->b04 = 3;
            } else {
                o->w08 = 0;
                o->step++;
            }
            break;
        case 1:
            o->w08 = 0;
            W1C(o) = D_80135EC0[FUN_8001f9e0() & 3];
            o->w1e = D_80135EC8[FUN_8001f9e0() & 3];
            o->step++;
            break;
        case 2:
            o->w08 += W1C(o);
            if (o->w08 < -0x300) o->w08 = -0x300;
            if ((unsigned short)--o->w1e == 0xffff) {
                o->w1e = 0x32;
                o->w08 = 0;
                o->step++;
            }
            break;
        case 3:
            if ((unsigned short)--o->w1e == 0xffff) o->step = 1;
            break;
        }
        GVARS.s46 = 0;
        if (pl->b69 == 1) break;
        if (U8(pl, 0xc9))
            v = o->w08 >> 1;
        else
            v = o->w08;
        if ((unsigned short)(pl->a.p.whole - 0x514) < 0xb4) {
            if (pl->b9c && pl->velY > 0x200) pl->velY = 0x200;
            GVARS.s46 = v;
        }
        if ((unsigned short)(pl->a.p.whole - 0x1012) < 0x226) {
            if (pl->b9c && pl->velY > 0x200) pl->velY = 0x200;
            GVARS.s46 = v;
        }
        if ((unsigned short)(pl->a.p.whole - 0x13a3) < 0x85) {
            if (pl->b9c && pl->velY > 0x200) pl->velY = 0x200;
            GVARS.s46 = v;
        }
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}
