// FUNC 801165e0 1288 X010
// MATCHING 801165e0 1288
#include "gcd.h"
#include "gvars.h"
#include "gamestate.h"
typedef struct { unsigned short frac; short whole; } FP;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0xa2 - 8];
    unsigned char ba2;
} PL;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
} O;
typedef void (*Fn)(O *);

extern PL PLAYER;
extern Fn D_80079A98[];
extern FP *D_800A6078;
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern unsigned int stepScrollReturnY(O *);
extern unsigned int stepScrollClearX(O *);
extern void cameraFollowPlayerX(O *);
extern void cameraFollowPlayerY(O *);
extern void syncObjectTargetY(O *);
extern void func_80029078(O *);
extern void func_80027810(O *);
extern int stepPlayerXToward(int);
extern int tryStartItemInteraction(O *, int, int);
extern void FUN_8002a4d0(O *);

#define XU (*(unsigned short *)&D_1F80016A)
#define ZU (*(unsigned short *)&D_1F80016E)

void updateAreaScroll(O *o)
{
    PL *p = &PLAYER;

    switch (o->step) {
    case 0:
        if (((unsigned char*)&GAME.disableSelectMenu)[0] != 0) {
            if (stepScrollReturnY(o) & stepScrollClearX(o)) ((unsigned char*)&GAME.disableSelectMenu)[0] = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        break;
    case 1:
        if (GVARS.b3a != 0) o->step++;
        cameraFollowPlayerX(o);
        cameraFollowPlayerY(o);
        syncObjectTargetY(o);
        func_80029078(o);
        break;
    case 2:
        if (GAME.selectedSection == 8) func_80027810(o);
        else D_80079A98[o->subtype](o);
        switch (GAME.selectedSection) {
        case 0:
        case 4:
            if (p->ba2 == 2) {
                short *y = &D_1F800172;
                if (*y < 0x5a) break;
                if ((unsigned)(XU - 0x56e) >= 0x60) break;
                stepPlayerXToward(0x59e);
                if (*y < 0x88) break;
                tryStartItemInteraction(o, 1, 0);
                break;
            }
            if (GCD.bd078 < 2) break;
            if (D_1F80016A < 0xc8b) break;
            if (D_1F80016E < -0x60) break;
            tryStartItemInteraction(o, 1, 1);
            break;
        case 1:
        case 5:
            if (PLAYER.ba2 == 2) {
                if (D_1F800172 >= -0x46) break;
                if (D_1F80016A < 0x21c) {
                    tryStartItemInteraction(o, 1, 0);
                    break;
                }
                if (D_1F80016A < 0x30d) break;
                tryStartItemInteraction(o, 1, 1);
                break;
            }
            if (GAME.purifiedAreas & 0x40) {
                if (D_1F80016A < 0x487) break;
                if (D_1F80016E < -0x171) break;
                tryStartItemInteraction(o, 1, 2);
                break;
            }
            if (D_1F80016A < 0x44d) break;
            D_800A6078->whole = 0x44c;
            break;
        case 2:
        case 6:
            if (D_1F80016A < 0x81 && D_1F80016E >= -0x40 && tryStartItemInteraction(o, 1, 0)) GVARS.b3e = 1;
            break;
        case 3:
        case 7:
            if ((unsigned)(XU - 0x53f) < 0x40) {
                if (GAME.purifiedAreas & 0x40) {
                    if ((unsigned short)(ZU + 0x392) < 0x1e) {
                        tryStartItemInteraction(o, 1, 1);
                        break;
                    }
                } else {
                    if ((unsigned short)(ZU + 0x434) < 0x1e) {
                        tryStartItemInteraction(o, 1, 1);
                        break;
                    }
                }
            }
            if (D_1F80016A < 0x14 && D_1F80016E >= -0x40f) {
                tryStartItemInteraction(o, 1, 0);
                break;
            }
            if (D_1F80016A < 0x7c2) break;
            if ((unsigned short)(ZU + 0x3fa) < 0x64) {
                tryStartItemInteraction(o, 1, 2);
                break;
            }
            if (D_1F80016A < 0x7c7) break;
            if ((unsigned short)(ZU + 0xe8) < 0x50) tryStartItemInteraction(o, 1, 3);
            break;
        case 8:
            if (D_1F80016A < 0x3a && D_1F80016E >= -0x3b) {
                if (GAME.purifiedAreas & 0x40) tryStartItemInteraction(o, 1, 1);
                else tryStartItemInteraction(o, 1, 0);
            }
            break;
        }
        break;
    case 3:
        FUN_8002a4d0(o);
        break;
    }
}
