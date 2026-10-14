// FUNC 8001a940 472 MAIN0
// MATCHING 8001a940 472
#include "globals.h"
extern int DAT_1f800164;
extern short DAT_1f8001f4;
extern short DAT_1f8001c6;
extern unsigned char DAT_1f8001cc;
extern unsigned char DAT_1f8003d0;
extern unsigned char DAT_1f8001cd;
extern unsigned short DAT_1f8001fc;
extern unsigned char DAT_1f8001d3;
extern unsigned short DAT_1f8001f8;
extern void FUN_8001ca58(void);
extern void updateAreaScreenEffect(void);
extern void tickHud(void);
extern void FUN_80039580(void);
extern void FUN_8001d36c(void);
extern void updateObjectsLayer7(void);
extern void updateObjectsUnlayered(void);
extern void FUN_80047af0(void);
extern void updateInventoryOverlay(void);
extern void FUN_8002af10(void);
extern void updateObjectsLayer8(void);
extern void resetDrawLists(void);
extern void drawGameplayScene(void);
extern void FUN_8001dbdc(void);

void gameplayTick(void)
{
    unsigned char c;
    DAT_1f800164 = (int)&LOAD_BUFFER_ALT[DAT_1f8001f4 * 0xc000] & 0xffffff;
    if (DAT_1f8001c6 == 2) {
        if (DAT_1f8001cc == 0) {
            DAT_1f8001c6 = 0;
        } else if (DAT_1f8003d0 == 1) {
            c = DAT_1f8001cd;
            if ((c == 5 || (unsigned char)(c - 7) < 2) && (DAT_1f8001fc & 0x4008)) {
                DAT_1f8001d3 = 1;
                DAT_1f8003d0 = 0;
            }
        }
    }
    FUN_8001ca58();
    if (DAT_1f8001c6 == 0) {
        DAT_1f8001f8++;
        updateAreaScreenEffect();
        tickHud();
        if (DAT_1f8001c6 == 0) {
            FUN_80039580();
            FUN_8001d36c();
            updateObjectsLayer7();
            updateObjectsUnlayered();
            FUN_80047af0();
        }
    }
    if (DAT_1f8001c6 != 1)
        updateInventoryOverlay();
    if (DAT_1f8001c6 == 0) {
        FUN_8002af10();
        updateObjectsLayer8();
    }
    if (DAT_1f8001c6 != 2)
        drawGameplayScene();
    else
        resetDrawLists();
    FUN_8001dbdc();
}
