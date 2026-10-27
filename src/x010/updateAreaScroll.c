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
extern FP *PLAYER_H;
extern short PLAYER_H_WHOLE, PLAYER_Y_WHOLE, PLAYER_D_WHOLE;
extern unsigned int stepScrollReturnY(O *);
extern unsigned int stepScrollClearX(O *);
extern void cameraFollowPlayerX(O *);
extern void cameraFollowPlayerY(O *);
extern void syncObjectTargetY(O *);
extern void updateCameraScroll(O *);
extern void cameraModeHandler6B(O *);
extern int stepPlayerXToward(int);
extern int tryStartItemInteraction(O *, int, int);
extern void cameraEventScrollHandler(O *);

#define XU (*(unsigned short *)&PLAYER_H_WHOLE)
#define ZU (*(unsigned short *)&PLAYER_Y_WHOLE)

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
        updateCameraScroll(o);
        break;
    case 2:
        if (GAME.selectedSection == 8) cameraModeHandler6B(o);
        else D_80079A98[o->subtype](o);
        switch (GAME.selectedSection) {
        case 0:
        case 4:
            if (p->ba2 == 2) {
                short *y = &PLAYER_D_WHOLE;
                if (*y < 0x5a) break;
                if ((unsigned)(XU - 0x56e) >= 0x60) break;
                stepPlayerXToward(0x59e);
                if (*y < 0x88) break;
                tryStartItemInteraction(o, 1, 0);
                break;
            }
            if (GCD.bd078 < 2) break;
            if (PLAYER_H_WHOLE < 0xc8b) break;
            if (PLAYER_Y_WHOLE < -0x60) break;
            tryStartItemInteraction(o, 1, 1);
            break;
        case 1:
        case 5:
            if (PLAYER.ba2 == 2) {
                if (PLAYER_D_WHOLE >= -0x46) break;
                if (PLAYER_H_WHOLE < 0x21c) {
                    tryStartItemInteraction(o, 1, 0);
                    break;
                }
                if (PLAYER_H_WHOLE < 0x30d) break;
                tryStartItemInteraction(o, 1, 1);
                break;
            }
            if (GAME.purifiedAreas & 0x40) {
                if (PLAYER_H_WHOLE < 0x487) break;
                if (PLAYER_Y_WHOLE < -0x171) break;
                tryStartItemInteraction(o, 1, 2);
                break;
            }
            if (PLAYER_H_WHOLE < 0x44d) break;
            PLAYER_H->whole = 0x44c;
            break;
        case 2:
        case 6:
            if (PLAYER_H_WHOLE < 0x81 && PLAYER_Y_WHOLE >= -0x40 && tryStartItemInteraction(o, 1, 0)) GVARS.b3e = 1;
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
            if (PLAYER_H_WHOLE < 0x14 && PLAYER_Y_WHOLE >= -0x40f) {
                tryStartItemInteraction(o, 1, 0);
                break;
            }
            if (PLAYER_H_WHOLE < 0x7c2) break;
            if ((unsigned short)(ZU + 0x3fa) < 0x64) {
                tryStartItemInteraction(o, 1, 2);
                break;
            }
            if (PLAYER_H_WHOLE < 0x7c7) break;
            if ((unsigned short)(ZU + 0xe8) < 0x50) tryStartItemInteraction(o, 1, 3);
            break;
        case 8:
            if (PLAYER_H_WHOLE < 0x3a && PLAYER_Y_WHOLE >= -0x3b) {
                if (GAME.purifiedAreas & 0x40) tryStartItemInteraction(o, 1, 1);
                else tryStartItemInteraction(o, 1, 0);
            }
            break;
        }
        break;
    case 3:
        cameraEventScrollHandler(o);
        break;
    }
}
