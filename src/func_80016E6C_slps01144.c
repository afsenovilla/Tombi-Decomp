// JP-only (slps01144), written from the disassembly
// VERSION SLPS_01144 slps01144 80016E6C 720
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
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
extern unsigned char DEBUG_KEEP_INVENTORY;
extern unsigned char DEBUG_GIVE_ITEMS;
extern short D_8009D2A4;
extern short DAT_1f8000ea;
extern short VIEW_CAMERA_X;
extern short VIEW_CAMERA_Y;
extern int RAND_SEED;
extern short DAT_1f8001c8;
extern short DAT_1f8000e2;
extern short VIEW_CAMERA_ROT_Y;
extern short DAT_1f8000f6;
extern unsigned char LOADED_SOUND_SET;
extern unsigned char D_8009C972;
extern unsigned char PAUSE_MENU_LOCK;
extern short SPAWN_AXIS_SWAP;
extern unsigned char WARP_FADE_ACTIVE;
extern unsigned short AREA_VARIANT;
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

void func_80016E6C_slps01144(void)
{
    register unsigned short cfg asm("$3");
    unsigned int c1b3; int one; int th;
    memset(((char*)&GAME.selectedArea), 0, 0x988);
    memset(((char*)&GVARS.b30), 0, 0x2c);
    getBaseMatrix(((char*)&SPAD.bc0));
    cfg = BUTTON_CONFIG_INDEX;
    asm("" : "=r"(cfg) : "0"(cfg));
    DAT_1f8000ea = -544;
    VIEW_CAMERA_X = 160;
    VIEW_CAMERA_Y = -120;
    th = 3; 
    DAT_1f8000e2 = 0;
    VIEW_CAMERA_ROT_Y = 0;
    DAT_1f8000f6 = 0;
    *(unsigned short *)((char*)&GAME.selectedArea) = 0;
    asm volatile("" ::: "memory"); c1b3 = DEBUG_KEEP_INVENTORY; asm volatile(""); one = 1;
    asm("" : "=r"(c1b3), "=r"(one) : "0"(c1b3), "1"(one));
    GAME.playerLives = th;
    GAME.saveSlot = 255;
    D_8009C972 = 9;
    GAME.playerMaxHealth = 4;
    GAME.playerHealth = 4;
    GCD.scfd0 = 1498;
    GCD.scfd2 = 1593;
    (*(short*)&GAME.selectedSection) = 0;
    GAME.inventoryScreen = 0;
    D_8009C973 = one;
    GCD.bceaf = one;
    SCRIPT_FLAGS = one;
    GCD.scfd4 = 1681;
    GCD.bcf1c = cfg;
    if (c1b3 == 0) {
        if (DEBUG_GIVE_ITEMS != 0) {
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
            GCD.bd0bf = one;
            GCD.bd0c5 = one;
            GCD.bd0c6 = one;
            GCD.bd0b9 = one;
            GCD.bd0ba = one;
            GCD.bd0bb = one;
            GCD.bd0bc = one;
            GCD.bd0c4 = one;
            GCD.bd0c1 = one;
            GCD.bd0c0 = one;
            GCD.bd0a9 = one;
            GCD.bd0aa = one;
            GCD.bd0c8 = one;
            GCD.bd0b8 = one;
            GV2.sa6 = 0x8000;
        } else {
            D_8009D1A4 = 27;
            D_8009D1A5 = 32;
            D_8009D2A4 = 2;
            GCD.bd0bf = one;
            GCD.bd0c4 = one;
            GV2.sa6 = 0x8000;
        }
    }
    DAT_1f8001c8 = 0;
    RAND_SEED = 69;
}
#endif
