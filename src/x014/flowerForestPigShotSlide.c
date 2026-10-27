// FUNC 80121f90 1160 X014
// MATCHING 80121f90 1160
#include "camera.h"
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
typedef struct { short s0, s2; } S2;
extern TObj PLAYER;
extern S2 *PLAYER_H;
extern unsigned char PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP, D_800A60D6;
extern short PLAYER_ANIM_FRAME;
extern short func_80040278(TObj *, short, short);

void flowerForestPigShotSlide(TObj *o)
{
    TObj *pl;
    switch (o->step) {
    case 0:
        o->velH = 0x80;
        o->timer = 0x1e;
        o->step++;
        break;
    case 1:
    case 3:
    case 6:
        if (--o->timer == -1) o->step++;
        break;
    case 2:
        if (GVARS.b3a != 0) {
            o->step++;
            if (o->subtype == 3) o->timer = 10;
            else o->timer = 0x1fe;
        }
        break;
    case 4:
        o->a.raw -= o->velH << 8;
        if (o->h->p.whole < CAMERA_SCROLL.minX - 0x96) {
            if (o->subtype == 3) {
                pl = &PLAYER;
                if (D_800A60D6 == 3) {
                    pl->active = 2;
                    PLAYER_STATE = 2;
                    PLAYER_SUBSTATE = 0;
                    PLAYER_STEP = 0;
                    PLAYER_ANIM_FRAME = o->h->p.whole > PLAYER_H->s2;
                }
            }
            o->step++;
        } else {
            if (o->h->p.whole < CAMERA_SCROLL.maxX + 0xa0) {
                short y = o->y.p.whole + 4;
                o->y.p.whole = y;
                if (func_80040278(o, o->h->p.whole, (short)(y + o->box2))) {
                    o->wb2 = (*(unsigned short*)&SPAD.s27e);
                    goto d4;
                }
            }
            o->wb2 = 0;
        }
    d4:
        if (o->wb2 != 0) {
            if (o->wb2 < 0) {
                o->velH += 0x20;
                if (o->velH > 0x200) o->velH = 0x200;
            } else {
                o->velH -= 0x10;
                if (o->velH < 0x80) o->velH = 0x80;
            }
        }
        o->d8c -= o->velH >> 5;
        break;
    case 5:
        o->a.raw -= o->velH << 8;
        o->d8c -= o->velH >> 5;
        if (o->a.p.whole < 0x22) {
            o->timer = 0x14;
            o->step++;
        }
        break;
    case 7:
        o->a.raw += o->velH << 8;
        if (CAMERA_SCROLL.maxX + 0x96 < o->h->p.whole) {
            if (o->subtype == 3) {
                pl = &PLAYER;
                if (D_800A60D6 == 3) {
                    pl->active = 2;
                    PLAYER_STATE = 2;
                    PLAYER_SUBSTATE = 0;
                    PLAYER_STEP = 0;
                    PLAYER_ANIM_FRAME = o->h->p.whole > PLAYER_H->s2;
                }
            }
            o->step++;
        } else {
            if (CAMERA_SCROLL.minX - 0xa0 < o->h->p.whole) {
                short y = o->y.p.whole + 4;
                o->y.p.whole = y;
                if (func_80040278(o, o->h->p.whole, (short)(y + o->box2))) {
                    o->wb2 = (*(unsigned short*)&SPAD.s27e);
                    goto d7;
                }
            }
            o->wb2 = 0;
        }
    d7:
        o->d8c += o->velH >> 5;
        if (o->wb2 != 0) {
            if (o->wb2 > 0) {
                o->velH += 0x20;
                if (o->velH > 0x200) o->velH = 0x200;
            } else {
                o->velH -= 0x10;
                if (o->velH < 0x80) o->velH = 0x80;
            }
        }
        break;
    case 8:
        o->a.raw += o->velH << 8;
        o->d8c += o->velH >> 5;
        if (o->a.p.whole >= 0x39f) {
            o->step = 3;
            o->timer = 0x14;
        }
        break;
    }
}
