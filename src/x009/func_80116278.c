// FUNC 80116278 808 X009
// MATCHING 80116278 808
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "gamestate.h"
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x30 - 8];
    short w30, w32;
    char p34[0x3c - 0x34];
    unsigned char b3c;
} O;
typedef void (*Fn)(O *);

extern Fn D_80079A98[];
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern short D_1F8000F2;
extern unsigned int stepScrollReturnY(O *);
extern unsigned int stepScrollClearX(O *);
extern void FUN_80027c74(O *);
extern void syncObjectTargetY(O *);
extern void func_80029078(O *);
extern int stepPlayerXToward(int);
extern int tryStartItemInteraction(O *, int, int);

#define XU (*(unsigned short *)&D_1F80016A)

void func_80116278(O *o)
{
    switch (o->step) {
    case 0:
        if (((unsigned char*)&GAME.disableSelectMenu)[0] != 0) {
            if (stepScrollReturnY(o) & stepScrollClearX(o)) ((unsigned char*)&GAME.disableSelectMenu)[0] = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        GVARS.b3d = 1;
        if (GAME.selectedSection == 0 && GAME.selectedSpawnPoint == 5)
            GVARS.b3a = 1;
        break;
    case 1:
        if (GVARS.b3a != 0) o->step++;
        FUN_80027c74(o);
        o->b3c = 0;
        syncObjectTargetY(o);
        func_80029078(o);
        break;
    case 2:
        D_80079A98[o->subtype](o);
        if (GCD.bcfee == 1 && (*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 2 && (unsigned)(XU - 0xb44) < 0x60) {
            stepPlayerXToward(0xb74);
            if (D_1F800172 >= 0xbb9) {
                tryStartItemInteraction(o, 1, 3);
                break;
            }
        }
        if (D_1F80016A < 0x4b0 && D_1F80016E >= -0x2cf)
            tryStartItemInteraction(o, 1, 0);
        else if (D_1F800172 == 0xb40 && D_1F80016A >= 0x11d5 && D_1F80016E >= -0x1d5)
            tryStartItemInteraction(o, 1, 2);
        else if ((unsigned)(XU - 0x1050) < 0x20 && D_1F800172 < 0xaf0)
            tryStartItemInteraction(o, 1, 1);
        if (D_1F800172 < 0xb36) {
            short *k = &D_1F8000F2;
            *k += 2;
            if (o->w32 < *k)
                *k = o->w32;
        }
        break;
    case 3:
        D_80079A98[o->subtype](o);
        break;
    }
}
