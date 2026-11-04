// FUNC 8001a940 472 MAIN0
// MATCHING 8001a940 472
// VERSION SLPS_01144 slps01144 8001A8CC 452
#if defined(VERSION_SLPS_01144)
#include "globals.h"
extern int NEXT_PRIM;
extern short FRAME_BUFFER_INDEX;
extern short DAT_1f8001c6;
extern unsigned char MOVIE_PLAY_STATE;
extern unsigned short TICK_COUNT;
extern unsigned char GAME_INVENTORY_SCREEN, MENU_STATE[];
extern unsigned short D_800A6132[], D_800A6134[], D_800A6136[];
extern unsigned short DAT_1f8003b8, DAT_1f8003ba;
typedef struct { char p[0x4c]; unsigned short w4c, w4e; } Q;
#include "spad.h"
extern void func_8001C2D8(void);
extern void updateAreaScreenEffect(void);
extern void func_800272BC(void);
extern void tickAreaScript(void);
extern void updateObjectsLayer7(void);
extern void updateMainObjects(void);
extern void func_8002BEC4_slps01144(void);
extern void updateObjectsLayer8(void);
extern void updateObjectsUnlayered(void);
extern void func_80052CF0(void);
extern void updateInventoryOverlay(void);
extern void func_80043A90(void);
extern void resetDrawLists(void);
extern void func_8001DAF0(void);
void func_8001A8CC(void)
{
    NEXT_PRIM = (int)&LOAD_BUFFER_ALT[FRAME_BUFFER_INDEX * 0xc000] & 0xffffff;
    if (DAT_1f8001c6 == 2) {
        if (MOVIE_PLAY_STATE == 0) {
            DAT_1f8001c6 = 0;
        }
    }
    func_8001C2D8();
    {
        unsigned char *inv = &GAME_INVENTORY_SCREEN;
        asm("" : "=r"(inv) : "0"(inv));
        if (*inv == 0xff) {
            Q *q = *(Q **)&SPAD._pad1d4[0];
            unsigned short x, y;
            *inv = 0;
            x = q->w4c;
            y = q->w4e;
            D_800A6132[0] = 6;
            D_800A6134[0] = 0;
            D_800A6136[0] = 0;
            MENU_STATE[0] = 0;
            q->w4c = 3;
            q->w4e = 0;
            DAT_1f8003b8 = x;
            DAT_1f8003ba = y;
        }
    }
    if (DAT_1f8001c6 == 0) {
        TICK_COUNT++;
        updateAreaScreenEffect();
        func_800272BC();
        if (DAT_1f8001c6 == 0) {
            tickAreaScript();
            updateObjectsLayer7();
            updateMainObjects();
            func_8002BEC4_slps01144();
            updateObjectsLayer8();
            updateObjectsUnlayered();
            func_80052CF0();
        }
    }
    if (DAT_1f8001c6 != 1)
        updateInventoryOverlay();
    if (DAT_1f8001c6 != 2)
        func_80043A90();
    else
        resetDrawLists();
    func_8001DAF0();
}
#else
#include "globals.h"
extern int NEXT_PRIM;
extern short FRAME_BUFFER_INDEX;
extern short DAT_1f8001c6;
extern unsigned char MOVIE_PLAY_STATE;
extern unsigned char DAT_1f8003d0;
extern unsigned char MOVIE_ID;
extern unsigned short JOYPAD_STATE;
extern unsigned char MOVIE_SKIP_REQUEST;
extern unsigned short TICK_COUNT;
extern void FUN_8001ca58(void);
extern void updateAreaScreenEffect(void);
extern void tickHud(void);
extern void tickAreaScript(void);
extern void FUN_8001d36c(void);
extern void updateObjectsLayer7(void);
extern void updateObjectsUnlayered(void);
extern void processObjectCollisions(void);
extern void updateInventoryOverlay(void);
extern void callAreaModelLinkHook(void);
extern void updateObjectsLayer8(void);
extern void resetDrawLists(void);
extern void drawGameplayScene(void);
extern void FUN_8001dbdc(void);

void gameplayTick(void)
{
    unsigned char c;
    NEXT_PRIM = (int)&LOAD_BUFFER_ALT[FRAME_BUFFER_INDEX * 0xc000] & 0xffffff;
    if (DAT_1f8001c6 == 2) {
        if (MOVIE_PLAY_STATE == 0) {
            DAT_1f8001c6 = 0;
        } else if (DAT_1f8003d0 == 1) {
            c = MOVIE_ID;
            if ((c == 5 || (unsigned char)(c - 7) < 2) && (JOYPAD_STATE & 0x4008)) {
                MOVIE_SKIP_REQUEST = 1;
                DAT_1f8003d0 = 0;
            }
        }
    }
    FUN_8001ca58();
    if (DAT_1f8001c6 == 0) {
        TICK_COUNT++;
        updateAreaScreenEffect();
        tickHud();
        if (DAT_1f8001c6 == 0) {
            tickAreaScript();
            FUN_8001d36c();
            updateObjectsLayer7();
            updateObjectsUnlayered();
            processObjectCollisions();
        }
    }
    if (DAT_1f8001c6 != 1)
        updateInventoryOverlay();
    if (DAT_1f8001c6 == 0) {
        callAreaModelLinkHook();
        updateObjectsLayer8();
    }
    if (DAT_1f8001c6 != 2)
        drawGameplayScene();
    else
        resetDrawLists();
    FUN_8001dbdc();
}
#endif
