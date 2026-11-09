// VERSION SCUS_94236 scus94236 800177D8 776
#if defined(VERSION_SCUS_94236)
// USA version of PAL initGameConfig.c (src/initGameConfig.c): VIEW_CAMERA_Y = -128;=>VIEW_CAMERA_Y = -120;.
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
extern short D_8009C60C;
extern short DAT_1f8000ea;
extern short VIEW_CAMERA_X;
extern short VIEW_CAMERA_Y;
extern int GAME_RAND_SEED;
extern short DAT_1f8000e2;
extern short VIEW_CAMERA_ROT_Y;
extern short DAT_1f8000f6;
extern unsigned char LOADED_SOUND_SET;
extern unsigned char D_8009BCDA;
extern unsigned char PAUSE_MENU_LOCK;
extern short SPAWN_AXIS_SWAP;
extern unsigned char WARP_FADE_ACTIVE;
extern unsigned short AREA_VARIANT;
extern unsigned char D_8009BCDB;
extern unsigned char SCRIPT_FLAGS;
extern unsigned char D_8009C50C;
extern unsigned char D_8009C50D;
extern unsigned char D_8009C50E;
extern unsigned char D_8009C50F;
extern unsigned char D_8009C510;
extern unsigned char D_8009C511;
extern unsigned char D_8009C512;
extern unsigned char D_8009C513;
extern unsigned char D_8009C514;
extern unsigned char D_8009C515;
extern unsigned char D_8009C516;
extern unsigned char D_8009C517;
extern unsigned char D_8009C518;
extern unsigned char D_8009C519;
extern void *memset(void *, int, int);
extern void getBaseMatrix(void *);

void initGameConfig(void)
{
    memset(((char*)&GAME.selectedArea), 0, 0x988);
    memset(((char*)&GVARS.b30), 0, 0x2c);
    getBaseMatrix(((char*)&SPAD.bc0));
    DAT_1f8000ea = -544;
    VIEW_CAMERA_X = 160;
    VIEW_CAMERA_Y = -120;
    GAME_RAND_SEED = 69;
    DAT_1f8000e2 = 0;
    VIEW_CAMERA_ROT_Y = 0;
    DAT_1f8000f6 = 0;
    LOADED_LIST_FILE_SYM = 255;
    LOADED_SOUND_SET = 255;
    (*(int*)&GAME.playerAP) = 0;
    *(unsigned short *)((char*)&GAME.selectedArea) = 0;
    GAME.playerLives = 3;
    GAME.saveSlot = 255;
    GCD.bd090 = 255;
    GCD.bcf1c = BUTTON_CONFIG_INDEX;
    PAUSE_MENU_LOCK = 0;
    SPAWN_AXIS_SWAP = 0;
    WARP_FADE_ACTIVE = 0;
    AREA_VARIANT = 0;
    (*(short*)&GAME.selectedSection) = 0;
    GAME.inventoryScreen = 0;
    D_8009BCDB = 1;
    GCD.bceaf = 1;
    SCRIPT_FLAGS = 1;
    D_8009BCDA = 9;
    GAME.playerMaxHealth = 4;
    GAME.playerHealth = 4;
    GCD.scfd0 = 1498;
    GCD.scfd2 = 1593;
    GCD.scfd4 = 1681;
    if (DEBUG_KEEP_INVENTORY == 0) {
        if (DEBUG_GIVE_ITEMS != 0) {
            D_8009C50C = 27;
            D_8009C50D = 33;
            D_8009C50E = 34;
            D_8009C50F = 21;
            D_8009C510 = 22;
            D_8009C511 = 23;
            D_8009C512 = 32;
            D_8009C513 = 29;
            D_8009C514 = 24;
            D_8009C515 = 28;
            D_8009C516 = 5;
            D_8009C517 = 6;
            D_8009C518 = 36;
            D_8009C519 = 20;
            D_8009C60C = 14;
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
            D_8009C50C = 27;
            D_8009C50D = 32;
            D_8009C60C = 2;
            GCD.bd0bf = 1;
            GCD.bd0c4 = 1;
            GV2.sa6 = 0x8000;
        }
    }
}
#endif
