// FUNC 80115f88 840 X016
// MATCHING 80115f88 840
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
extern unsigned char D_8009D000[];
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern short D_1F8000F2[];
extern int D_1F8000F0;
extern void func_80027810(O *);
extern int tryStartItemInteraction(O *, int, int);
extern void FUN_8002a4d0(O *);

void func_80115F88(O *o)
{
    PL *p = &PLAYER;

    switch (o->step) {
    case 0:
        if (GVARS.b3a != 0) o->step++;
        func_80027810(o);
        o->p38->whole = 0;
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        switch (GAME.selectedSection) {
        case 1:
        case 4:
            if (D_1F800172 >= 0xc9) {
                if (D_8009D000[0] == 0) {
                    D_8009D000[0] = 1;
                    p->b04 = 5;
                    p->step = 0x41;
                    p->visible = 0;
                    p->state = 0;
                    o->step = 3;
                    break;
                }
                tryStartItemInteraction(o, 1, 0);
            } else if (D_1F800172 < -0x59) {
                tryStartItemInteraction(o, 1, 1);
            }
            break;
        case 2:
        case 5:
            if (D_1F80016A < 0x46 && D_1F80016E >= -0x1d) {
                tryStartItemInteraction(o, 1, 0);
                break;
            }
            if (D_1F80016A < 0xfb) break;
            if (D_1F80016E < -0xd8) tryStartItemInteraction(o, 1, 1);
            break;
        case 3:
        case 6:
            if (D_1F80016A < 0x105) break;
            if (D_1F80016E < -0x1d) break;
            tryStartItemInteraction(o, 1, 0);
            break;
        }
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    case 3:
        D_1F8000F2[0] -= 2;
        if (D_1F8000F2[0] < o->w30) {
            D_1F8000F0 = o->w30 << 16;
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
        D_1F8000F2[0] += 2;
        if (o->w32 < D_1F8000F2[0]) D_1F8000F0 = o->w32 << 16;
        break;
    }
}
