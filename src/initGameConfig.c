// FUNC 8001783c 776 MAIN0
// MATCHING 8001783c 776
#include "spad.h"
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
#include "globals.h"
#include "gamestate.h"
/* Game-state init. No volatile: the store `*(unsigned short *)((char*)&GAME.selectedArea) = 0` through the memset pointer (kept in s0)
   is a varying NON-struct store, which sched2 orders against every fixed store (a barrier); GAME.playerLives = 3 placed
   inside the 0xff stores' range makes the 255 pseudo overlap the 3 pseudo so it gets v1; sched2 then emits the
   free stores in source order (ties by luid). */
extern unsigned short BUTTON_CONFIG_INDEX;
extern unsigned char DAT_1f8001b3;
extern unsigned char DAT_1f8001ab;
extern short D_8009D2A4;
extern short DAT_1f8000ea;
extern short DAT_1f8000ee;
extern short DAT_1f8000f2;
extern int DAT_1f800200;
extern short DAT_1f8000e2;
extern short DAT_1f8000e6;
extern short DAT_1f8000f6;
extern unsigned char DAT_1f8003d3;
extern unsigned char D_8009C972;
extern unsigned char DAT_1f8003ce;
extern short DAT_1f8001c8;
extern unsigned char DAT_1f8003d1;
extern short D_8009F838;
extern unsigned char D_8009C973;
extern unsigned char SCRIPT_FLAGS;
extern unsigned char D_8009D1A4;
extern unsigned char D_8009D1A5;
extern unsigned char D_8009D1A6;
extern unsigned char D_8009D1A7;
extern unsigned char D_8009D1A8;
extern unsigned char D_8009D1A9;
extern unsigned char D_8009D1AA;
extern unsigned char D_8009D1AB;
extern unsigned char D_8009D1AC;
extern unsigned char D_8009D1AD;
extern unsigned char D_8009D1AE;
extern unsigned char D_8009D1AF;
extern unsigned char D_8009D1B0;
extern unsigned char D_8009D1B1;
extern void *memset(void *, int, int);
extern void getBaseMatrix(void *);

void initGameConfig(void)
{
    memset(((char*)&GAME.selectedArea), 0, 0x988);
    memset(((char*)&GVARS.b30), 0, 0x2c);
    getBaseMatrix(((char*)&SPAD.bc0));
    DAT_1f8000ea = -544;
    DAT_1f8000ee = 160;
    DAT_1f8000f2 = -128;
    DAT_1f800200 = 69;
    DAT_1f8000e2 = 0;
    DAT_1f8000e6 = 0;
    DAT_1f8000f6 = 0;
    LOADED_LIST_FILE_SYM = 255;
    DAT_1f8003d3 = 255;
    (*(int*)&GAME.playerAP) = 0;
    *(unsigned short *)((char*)&GAME.selectedArea) = 0;
    GAME.playerLives = 3;
    GAME.saveSlot = 255;
    GCD.bd090 = 255;
    GCD.bcf1c = BUTTON_CONFIG_INDEX;
    DAT_1f8003ce = 0;
    DAT_1f8001c8 = 0;
    DAT_1f8003d1 = 0;
    D_8009F838 = 0;
    (*(short*)&GAME.selectedSection) = 0;
    GAME.inventoryScreen = 0;
    D_8009C973 = 1;
    GCD.bceaf = 1;
    SCRIPT_FLAGS = 1;
    D_8009C972 = 9;
    GAME.playerHealthDisplayed = 4;
    GAME.playerHealth = 4;
    GCD.scfd0 = 1498;
    GCD.scfd2 = 1593;
    GCD.scfd4 = 1681;
    if (DAT_1f8001b3 == 0) {
        if (DAT_1f8001ab != 0) {
            D_8009D1A4 = 27;
            D_8009D1A5 = 33;
            D_8009D1A6 = 34;
            D_8009D1A7 = 21;
            D_8009D1A8 = 22;
            D_8009D1A9 = 23;
            D_8009D1AA = 32;
            D_8009D1AB = 29;
            D_8009D1AC = 24;
            D_8009D1AD = 28;
            D_8009D1AE = 5;
            D_8009D1AF = 6;
            D_8009D1B0 = 36;
            D_8009D1B1 = 20;
            D_8009D2A4 = 14;
            GCD.bd0bf = 1;
            GCD.bd0c5 = 1;
            GCD.bd0c6 = 1;
            GCD.bd0b9 = 1;
            GCD.bd0ba = 1;
            GCD.bd0bb = 1;
            GCD.bd0bc = 1;
            GCD.bd0c4 = 1;
            GCD.bd0c1 = 1;
            GCD.bd0c0 = 1;
            GCD.bd0a9 = 1;
            GCD.bd0aa = 1;
            GCD.bd0c8 = 1;
            GCD.bd0b8 = 1;
            GV2.sa6 = 0x8000;
        } else {
            D_8009D1A4 = 27;
            D_8009D1A5 = 32;
            D_8009D2A4 = 2;
            GCD.bd0bf = 1;
            GCD.bd0c4 = 1;
            GV2.sa6 = 0x8000;
        }
    }
}
