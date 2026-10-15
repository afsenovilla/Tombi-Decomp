// FUNC 80115f88 840 X016
// MATCHING 80115f88 840
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
#include "gamestate.h"
#include "gametask.h"
typedef struct { unsigned short frac; short whole; } FP;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x30 - 8];
    short w30, w32;
    char p34[0x38 - 0x34];
    FP *p38;
    char p3c[0x48 - 0x3c];
    short w48;
} O;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
} PL;
typedef struct { char p[0x4c]; short w4c, w4e; } Q;
typedef void (*Fn)(O *);

extern PL PLAYER;
extern Fn D_80079AD0[];
extern void cameraModeHandler6B(O *);
extern int tryStartItemInteraction(O *, int, int);
extern void cameraEventScrollHandler(O *);

void updateAreaScroll(O *o)
{
    PL *p = &PLAYER;

    switch (o->step) {
    case 0:
        if (GVARS.b3a != 0) o->step++;
        cameraModeHandler6B(o);
        o->p38->whole = 0;
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        switch (GAME.selectedSection) {
        case 1:
        case 4:
            if ((*(short*)((char*)&SPAD.i170 + 2)) >= 0xc9) {
                if (((unsigned char*)&GCD.bd000)[0] == 0) {
                    ((unsigned char*)&GCD.bd000)[0] = 1;
                    p->b04 = 5;
                    p->step = 0x41;
                    p->visible = 0;
                    p->state = 0;
                    o->step = 3;
                    break;
                }
                tryStartItemInteraction(o, 1, 0);
            } else if ((*(short*)((char*)&SPAD.i170 + 2)) < -0x59) {
                tryStartItemInteraction(o, 1, 1);
            }
            break;
        case 2:
        case 5:
            if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x46 && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x1d) {
                tryStartItemInteraction(o, 1, 0);
                break;
            }
            if ((*(short*)((char*)&SPAD.i168 + 2)) < 0xfb) break;
            if ((*(short*)((char*)&SPAD.i16c + 2)) < -0xd8) tryStartItemInteraction(o, 1, 1);
            break;
        case 3:
        case 6:
            if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x105) break;
            if ((*(short*)((char*)&SPAD.i16c + 2)) < -0x1d) break;
            tryStartItemInteraction(o, 1, 0);
            break;
        }
        break;
    case 2:
        cameraEventScrollHandler(o);
        break;
    case 3:
        ((short*)((char*)&SPAD.if0 + 2))[0] -= 2;
        if (((short*)((char*)&SPAD.if0 + 2))[0] < o->w30) {
            SPAD.if0 = o->w30 << 16;
            o->w48 = 0x1e;
            o->step++;
        }
        break;
    case 4:
        if (--o->w48 == -1) {
            o->w48 = 0x3c;
            o->step++;
        }
        break;
    case 5:
        if (--o->w48 == -1) {
            (*(short*)&GAME.nextArea) = 0x10;
            (*(short*)&GAME.nextSection) = 5;
            (*(short*)&GAME.nextSpawnPoint) = 0;
            GVARS.b3c = 0;
            GAME.fadeScreenControl = 3;
            o->step++;
        }
    case 6:
        if (GAME.fadeScreenControl == 1) {
            TASK_C->mode = 7;
            TASK_C->step = 0;
        }
        ((short*)((char*)&SPAD.if0 + 2))[0] += 2;
        if (o->w32 < ((short*)((char*)&SPAD.if0 + 2))[0]) SPAD.if0 = o->w32 << 16;
        break;
    }
}
