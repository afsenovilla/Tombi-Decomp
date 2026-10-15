// FUNC 801165a0 1584 X009
// MATCHING 801165a0 1584
#include "gcd.h"
#include "gvars.h"
#include "gamestate.h"
typedef struct { short s0, s2; } S2;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x34 - 8];
    S2 *p34;
    char p38[0x4a - 0x38];
    short w4a;
} O;
typedef void (*Fn)(O *);
typedef struct { unsigned char b0, b1, b2, b3, b4, b5, b6; } PL;

extern PL PLAYER;
extern Fn D_80079A98[];
extern Fn D_80079AB4[];
extern unsigned char D_800A60DA;
extern short D_800A457C;
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern short D_1F8000EE, D_1F8000E6, D_1F8000F2;
extern void cameraModeHandler6(O *);
extern void stepPlayerXToward(int);
extern int tryStartItemInteraction(O *, int, unsigned char);
extern void setEventStarted(int, int, int);

void updateAreaScrollB(O *o)
{
    PL *pl = &PLAYER;

    switch (o->step) {
    case 0:
        if (GVARS.b3a) {
            o->step++;
            o->state = 0;
            o->subtype = 0;
        }
        cameraModeHandler6(o);
        break;
    case 1:
        D_80079AB4[o->subtype](o);
        switch (GAME.selectedSection) {
        case 1:
            switch (GAME.selectedSpawnPoint) {
            case 0:
                if (D_1F80016A >= 0x4b1 && D_1F80016E >= -0x59) {
                    o->state = 0;
                    tryStartItemInteraction(o, 1, 0);
                    break;
                }
                switch (o->state) {
                case 0:
                    if (D_1F80016A < 0x348) {
                        o->state++;
                        GVARS.b3c = 0;
                        GAME.fadeScreenControl = 3;
                    }
                    if (o->p34->s2 < 0x3bc) o->p34->s2 = 0x3bc;
                    break;
                case 1:
                    if (o->p34->s2 < 0x3bc) o->p34->s2 = 0x3bc;
                    if (GAME.fadeScreenControl == 1) o->state++;
                    break;
                case 2:
                    if (D_1F80016A < 0x276) {
                        o->state++;
                        GVARS.b3c = 0;
                        GAME.fadeScreenControl = 4;
                    }
                    if (o->p34->s2 > 0x1e0) o->p34->s2 = 0x1e0;
                    break;
                case 3:
                    if (D_1F80016A < 0xc8) {
                        if (GCD.bcdc0 == 0) {
                            pl->b4 = 5;
                            pl->b5 = 0;
                            pl->b6 = 0;
                            o->state++;
                        } else {
                            o->state = 0;
                            GAME.selectedSpawnPoint = 1;
                        }
                    }
                    if (o->p34->s2 > 0x1e0) o->p34->s2 = 0x1e0;
                    break;
                case 4: {
                    short *s = &D_1F8000EE;
                    *s -= 1;
                    if (*s < D_800A457C) {
                        *s = D_800A457C;
                        o->w4a = 0x1e;
                        o->state++;
                    }
                    D_1F8000E6 -= 2;
                    if (D_1F8000F2 + D_1F8000E6 < -0x156) {
                        D_1F8000E6 = -0x156 - D_1F8000F2;
                        o->w4a = 0x1e;
                        o->state++;
                    }
                    break;
                }
                case 5:
                    if (o->w4a) {
                        o->w4a--;
                        break;
                    } else {
                        short *s = &D_1F8000E6;
                        *s += 2;
                        if (*s > 0) {
                            *s = 0;
                            o->state = 0;
                            o->w4a = 0;
                            setEventStarted(0x1c, 0, 1);
                            GAME.selectedSpawnPoint = 1;
                        }
                    }
                    break;
                }
                break;
            case 1:
            case 2:
                if (D_800A60DA == 2) {
                    if (D_1F80016A >= 0x71) {
                        stepPlayerXToward(0x90);
                        GAME.selectedSpawnPoint = 1;
                    } else {
                        stepPlayerXToward(0x48);
                        GAME.selectedSpawnPoint = 2;
                    }
                    if (D_1F800172 >= 0x3d) tryStartItemInteraction(o, 1, GAME.selectedSpawnPoint);
                }
                if (o->p34->s2 > 0x1e0) o->p34->s2 = 0x1e0;
                break;
            }
            break;
        case 2:
            if (D_1F80016A >= 0x1c9 && D_1F80016E >= -0x39) tryStartItemInteraction(o, 1, 0);
            break;
        case 3:
            if (D_1F80016A < 0x14 && D_1F80016E >= -0x47) {
                tryStartItemInteraction(o, 1, 0);
            } else if (D_1F80016A >= 0x129 && D_1F80016E >= -0x47) {
                o->subtype = 1;
                tryStartItemInteraction(o, 1, 1);
            }
            break;
        case 4:
            if (D_1F80016A < 0x14 && D_1F80016E >= -0xdb) {
                tryStartItemInteraction(o, 1, 0);
            } else if (D_1F80016A >= 0x12a && D_1F80016E >= -0x21) {
                tryStartItemInteraction(o, 1, 1);
            }
            break;
        case 5:
            if (D_1F80016A < 0x14 && D_1F80016E >= -0x31) {
                tryStartItemInteraction(o, 1, 0);
            } else if (D_1F80016A >= 0x12d && D_1F80016E >= -0xd3) {
                tryStartItemInteraction(o, 1, 1);
            }
            break;
        }
        break;
    case 2:
        D_80079A98[o->subtype](o);
        break;
    }
}
