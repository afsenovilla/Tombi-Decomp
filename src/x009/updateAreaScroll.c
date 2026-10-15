// FUNC 80116278 808 X009
// MATCHING 80116278 808
// VERSION SLPS_01144 x09 80115534 808
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x09/nonmatchings/x09", func_80115534_slps01144);
#else
#include "spad.h"
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
extern short D_1F800172;
extern unsigned int stepScrollReturnY(O *);
extern unsigned int stepScrollClearX(O *);
extern void cameraFollowPlayerX(O *);
extern void syncObjectTargetY(O *);
extern void updateCameraScroll(O *);
extern int stepPlayerXToward(int);
extern int tryStartItemInteraction(O *, int, int);

#define XU (*(unsigned short *)&(*(short*)((char*)&SPAD.i168 + 2)))

void updateAreaScroll(O *o)
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
        cameraFollowPlayerX(o);
        o->b3c = 0;
        syncObjectTargetY(o);
        updateCameraScroll(o);
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
        if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x4b0 && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x2cf)
            tryStartItemInteraction(o, 1, 0);
        else if (D_1F800172 == 0xb40 && (*(short*)((char*)&SPAD.i168 + 2)) >= 0x11d5 && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x1d5)
            tryStartItemInteraction(o, 1, 2);
        else if ((unsigned)(XU - 0x1050) < 0x20 && D_1F800172 < 0xaf0)
            tryStartItemInteraction(o, 1, 1);
        if (D_1F800172 < 0xb36) {
            short *k = &(*(short*)((char*)&SPAD.if0 + 2));
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
#endif
