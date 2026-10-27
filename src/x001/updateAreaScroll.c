// FUNC 80116990 2816 X001
// MATCHING 80116990 2816
// FLAGS -O2 -G0 -fno-cse-skip-blocks
#include "gcd.h"
#include "gvars.h"
#include "gamestate.h"
#include "gametask.h"
#include "camera.h"
typedef struct { short s0, s2; } S2;
typedef struct { char p[0x4c]; short w4c, w4e; } SC;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x30 - 8];
    short w30, w32;
    char p34[0x3c - 0x34];
    unsigned char b3c;
    char p3d[0x48 - 0x3d];
    short w48, w4a;
} O;
typedef void (*Fn)(O *);
typedef struct { unsigned char b0, b1, b2, b3, b4, b5, b6; } PL;

extern PL PLAYER;
extern unsigned char D_800A6039[];
extern Fn D_80079A98[];
extern unsigned char D_800A60DA, D_800A60E0, D_800A60D8, PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern signed char GAME_PLAYER_MODE;
extern unsigned short D_800A6066;
extern S2 *D_800A6078x[];
#define D_800A6078 D_800A6078x[0]
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern unsigned int stepScrollReturnY(O *);
extern unsigned int stepScrollClearX(O *);
extern void cameraFollowPlayerX(O *);
extern void syncObjectTargetY(O *);
extern void updateCameraScroll(O *);
extern void markSectionVisited(void);
extern void stepPlayerXToward(int);
extern int tryStartItemInteraction(O *, int, unsigned char);
extern void playerSetAnim(PL *, int, int);
extern void AnimAdvance(PL *);
extern void setEventComplete(int, int);
extern void cameraModeHandler6(O *);

#define XU (*(unsigned short *)&D_1F80016A)
#define ZU (*(unsigned short *)&D_1F80016E)

void updateAreaScroll(O *o)
{
    switch (o->step) {
    case 0: {
        unsigned char *f = &GAME.disableSelectMenu;
        if (*f != 0) {
            if (stepScrollReturnY(o) & stepScrollClearX(o)) *f = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        break;
    }
    case 1:
        if (GVARS.b3a != 0) o->step++;
        cameraFollowPlayerX(o);
        o->b3c = 0;
        syncObjectTargetY(o);
        updateCameraScroll(o);
        break;
    case 2:
        D_80079A98[o->subtype](o);
        switch (GAME.selectedSection) {
        case 0: {
            unsigned short *p = &GAME.selectedSpawnPoint;
            if (*p != 0) {
                *p = 0;
                break;
            }
            if (D_1F80016A >= 0xb6e && D_1F80016E < -0x1d6) {
                markSectionVisited();
                GAME.selectedSection = 1;
                *p = 0;
                break;
            }
            if (D_1F80016A >= 0xb70 && D_1F80016E >= -0x118) {
                if (GCD.bcdb5 == 0) break;
                goto t11;
            }
            if (D_1F80016A >= 0x75) break;
            if (D_1F80016E < -0xc7) break;
            goto t10;
        }
        case 1: {
            unsigned short *p = &GAME.selectedSpawnPoint;
            short x;
            if (*p != 0) {
                *p = 0;
                break;
            }
            x = D_1F80016A;
            if (x < 0xb6d) {
                GAME.selectedSection = 0;
                markSectionVisited();
                *p = 0;
                break;
            }
            if (x >= 0x109b && D_1F80016E >= -0x179) goto t12;
            if (GCD.bcdb5 == 0) break;
            if (D_800A60DA != 2) break;
            if ((unsigned short)(XU - 0x102f) >= 0x60) break;
            stepPlayerXToward(0x105f);
            if (D_1F800172 < 0x51) break;
            goto t11;
        }
        case 2: {
            unsigned short *p = &GAME.selectedSpawnPoint;
            if (*p == 0) {
                if (D_1F80016A < 0x65) goto t10;
                if (D_1F80016A >= 0x779 && D_1F80016E >= -0x1c6) goto t11;
                if (GCD.bce58 == 0) break;
                if (D_1F80016A < 0x6e3) break;
                if ((unsigned short)(ZU + 0x46) >= 0x1e) break;
                goto t12;
            } else {
                *p = 0;
            }
            break;
        }
        case 3: {
            unsigned short *p = &GAME.selectedSpawnPoint;
            unsigned short k = *p;
            switch (k) {
            case 0:
                if (D_1F80016E < -0x5dc && (unsigned short)(XU - 0x50f) < 0x60 && D_800A60DA != 0) {
                    stepPlayerXToward(0x53f);
                    break;
                }
                if (GCD.bcdf1 == 0xff && (unsigned short)(XU - 0x69c) < 0x13 && D_1F80016E >= -0x29d) {
                    o->step = 4;
                    GVARS.b3f = 1;
                    GVARS.b42 = 1;
                    GCD.bcef7 = 0;
                    GAME_PLAYER_MODE = 0;
                    PLAYER.b0 = 5;
                    PLAYER_STATE = 5;
                    PLAYER_SUBSTATE = 0;
                    PLAYER_STEP = 0;
                    break;
                }
                if (D_1F80016A < 0x17e && D_1F80016E >= -0x18f) goto t10;
                if (D_1F80016A >= 0x7fc && D_1F80016E >= -0x293) {
                t11:
                    tryStartItemInteraction(o, 1, 1);
                    break;
                }
                if (D_1F80016A >= 0x821) GAME.selectedSpawnPoint = 2;
                break;
            case 1:
                *p = 0;
                break;
            case 2:
                if (D_1F80016A < 0x820) {
                    *p = 0;
                    break;
                }
                if (D_1F80016A < 0xb4b) break;
                if (D_1F80016E < -0x1d5) break;
                tryStartItemInteraction(o, 1, k);
                break;
            default:
                GAME.selectedSpawnPoint = 0;
                break;
            }
            break;
        }
        case 4: {
            unsigned short *p = &GAME.selectedSpawnPoint;
            if (*p == 0) {
                if (GAME.playerAP > 999999 && GCD.bce48 != 0 && (unsigned short)(XU - 0xb5c) < 0x20) {
                    D_800A60E0 = 8;
                    D_800A60D8 = 0x21;
                    if (D_1F800172 >= 0x881) {
                        stepPlayerXToward(0xb6c);
                        if (D_1F800172 >= 0x8d1) {
                            o->step = 8;
                            PLAYER_STATE = 5;
                            D_800A6039[0] = 0;
                            PLAYER_SUBSTATE = 0x41;
                            PLAYER_STEP = 0;
                            break;
                        }
                    }
                }
                if (D_1F80016A >= 0xfab && D_1F80016E >= -0x1cb) {
                t10:
                    tryStartItemInteraction(o, 1, 0);
                    break;
                }
                if ((unsigned short)(XU - 0x72e) < 0x20 && D_1F800172 >= 0x881) {
                    stepPlayerXToward(0x73e);
                    if (D_1F800172 >= 0x895) {
                    t12:
                        tryStartItemInteraction(o, 1, 2);
                        break;
                    }
                }
                if ((unsigned short)(XU - 0xb28) < 0x80 && D_1F800172 < 0x880) {
                    stepPlayerXToward(0xb68);
                    if (D_1F800172 < 0x80c) tryStartItemInteraction(o, 1, 1);
                }
                if (D_1F800172 < 0x87a) {
                    short *q = &VIEW_CAMERA.pos[1].p.hi;
                    *q += 2;
                    if (o->w32 < *q) *q = o->w32;
                }
                break;
            } else {
                *p = 0;
            }
            break;
        }
        }
        break;
    case 3:
        if (GVARS.b3f == 0) D_80079A98[o->subtype](o);
        break;
    case 4: {
        unsigned char *f = &PLAYER.b6;
        if (*f == 2) {
            playerSetAnim(&PLAYER, 1, 0);
            PLAYER_STATE = 5;
            PLAYER_SUBSTATE = 0x41;
            *f = 0;
            D_800A6066 = 1;
            if (GCD.bcdbc == 0xff) {
                o->step = 6;
                o->w4a = 0;
            } else {
                o->step++;
            }
        }
        cameraModeHandler6(o);
        break;
    }
    case 5:
        AnimAdvance(&PLAYER);
        D_800A6078->s2--;
        if (D_800A6078->s2 < 0x6ae) {
            o->step++;
            setEventComplete(0x18, 0);
            o->w4a = 0x154;
        }
        cameraModeHandler6(o);
        break;
    case 6: {
        PL *pl;
        if (o->w4a == 0) {
            pl = &PLAYER;
            AnimAdvance(pl);
            D_800A6078->s2--;
            if (D_800A6078->s2 < 0x67c) {
                o->step++;
                playerSetAnim(pl, 0, 0);
                o->w4a = 0x3c;
            }
            cameraModeHandler6(o);
        } else {
            o->w4a--;
        }
        break;
    }
    case 7:
        D_800A6039[0] = 0;
        if (--o->w4a == -1) {
            tryStartItemInteraction(o, 1, 3);
            o->step = 3;
            GVARS.b3f = 0;
        }
        break;
    case 8:
        o->w48 = 0x3c;
        o->step++;
        break;
    case 9:
        if (--o->w48 == -1) {
            (*(short*)&GAME.nextArea) = 0xd;
            (*(short*)&GAME.nextSection) = 0;
            (*(short*)&GAME.nextSpawnPoint) = 0;
            GVARS.b3c = 0;
            GAME.fadeScreenControl = 3;
            o->step++;
        }
    case 10: {
        short *q;
        if (GAME.fadeScreenControl == 1) {
            TASK_C->mode = 7;
            TASK_C->step = 0;
        }
        q = &VIEW_CAMERA.pos[1].p.hi;
        *q -= 2;
        if (*q < o->w30) VIEW_CAMERA.pos[1].raw = o->w30 << 16;
        break;
    }
    }
}
