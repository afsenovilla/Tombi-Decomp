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
extern unsigned char PLAYER_VISIBLE[];
extern Fn D_80079A98[];
extern unsigned char D_800A60DA, D_800A60E0, D_800A60D8, PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern signed char GAME_PLAYER_MODE;
extern unsigned short PLAYER_ANIM_FRAME;
extern S2 *D_800A6078x[];
#define D_800A6078 D_800A6078x[0]
extern short PLAYER_H_WHOLE, PLAYER_Y_WHOLE, PLAYER_D_WHOLE;
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

#define XU (*(unsigned short *)&PLAYER_H_WHOLE)
#define ZU (*(unsigned short *)&PLAYER_Y_WHOLE)

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
            if (PLAYER_H_WHOLE >= 0xb6e && PLAYER_Y_WHOLE < -0x1d6) {
                markSectionVisited();
                GAME.selectedSection = 1;
                *p = 0;
                break;
            }
            if (PLAYER_H_WHOLE >= 0xb70 && PLAYER_Y_WHOLE >= -0x118) {
                if (GCD.bcdb5 == 0) break;
                goto t11;
            }
            if (PLAYER_H_WHOLE >= 0x75) break;
            if (PLAYER_Y_WHOLE < -0xc7) break;
            goto t10;
        }
        case 1: {
            unsigned short *p = &GAME.selectedSpawnPoint;
            short x;
            if (*p != 0) {
                *p = 0;
                break;
            }
            x = PLAYER_H_WHOLE;
            if (x < 0xb6d) {
                GAME.selectedSection = 0;
                markSectionVisited();
                *p = 0;
                break;
            }
            if (x >= 0x109b && PLAYER_Y_WHOLE >= -0x179) goto t12;
            if (GCD.bcdb5 == 0) break;
            if (D_800A60DA != 2) break;
            if ((unsigned short)(XU - 0x102f) >= 0x60) break;
            stepPlayerXToward(0x105f);
            if (PLAYER_D_WHOLE < 0x51) break;
            goto t11;
        }
        case 2: {
            unsigned short *p = &GAME.selectedSpawnPoint;
            if (*p == 0) {
                if (PLAYER_H_WHOLE < 0x65) goto t10;
                if (PLAYER_H_WHOLE >= 0x779 && PLAYER_Y_WHOLE >= -0x1c6) goto t11;
                if (GCD.bce58 == 0) break;
                if (PLAYER_H_WHOLE < 0x6e3) break;
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
                if (PLAYER_Y_WHOLE < -0x5dc && (unsigned short)(XU - 0x50f) < 0x60 && D_800A60DA != 0) {
                    stepPlayerXToward(0x53f);
                    break;
                }
                if (GCD.bcdf1 == 0xff && (unsigned short)(XU - 0x69c) < 0x13 && PLAYER_Y_WHOLE >= -0x29d) {
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
                if (PLAYER_H_WHOLE < 0x17e && PLAYER_Y_WHOLE >= -0x18f) goto t10;
                if (PLAYER_H_WHOLE >= 0x7fc && PLAYER_Y_WHOLE >= -0x293) {
                t11:
                    tryStartItemInteraction(o, 1, 1);
                    break;
                }
                if (PLAYER_H_WHOLE >= 0x821) GAME.selectedSpawnPoint = 2;
                break;
            case 1:
                *p = 0;
                break;
            case 2:
                if (PLAYER_H_WHOLE < 0x820) {
                    *p = 0;
                    break;
                }
                if (PLAYER_H_WHOLE < 0xb4b) break;
                if (PLAYER_Y_WHOLE < -0x1d5) break;
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
                    if (PLAYER_D_WHOLE >= 0x881) {
                        stepPlayerXToward(0xb6c);
                        if (PLAYER_D_WHOLE >= 0x8d1) {
                            o->step = 8;
                            PLAYER_STATE = 5;
                            PLAYER_VISIBLE[0] = 0;
                            PLAYER_SUBSTATE = 0x41;
                            PLAYER_STEP = 0;
                            break;
                        }
                    }
                }
                if (PLAYER_H_WHOLE >= 0xfab && PLAYER_Y_WHOLE >= -0x1cb) {
                t10:
                    tryStartItemInteraction(o, 1, 0);
                    break;
                }
                if ((unsigned short)(XU - 0x72e) < 0x20 && PLAYER_D_WHOLE >= 0x881) {
                    stepPlayerXToward(0x73e);
                    if (PLAYER_D_WHOLE >= 0x895) {
                    t12:
                        tryStartItemInteraction(o, 1, 2);
                        break;
                    }
                }
                if ((unsigned short)(XU - 0xb28) < 0x80 && PLAYER_D_WHOLE < 0x880) {
                    stepPlayerXToward(0xb68);
                    if (PLAYER_D_WHOLE < 0x80c) tryStartItemInteraction(o, 1, 1);
                }
                if (PLAYER_D_WHOLE < 0x87a) {
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
            PLAYER_ANIM_FRAME = 1;
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
        PLAYER_VISIBLE[0] = 0;
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
            GVARS.fadeEffect = 0;
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
