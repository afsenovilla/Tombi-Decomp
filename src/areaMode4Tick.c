// FUNC 8001b680 332 MAIN0
// MATCHING 8001b680 332
extern short G1f4, G1c6;
extern unsigned short G1f8;
extern unsigned int G164;
extern char LOAD_BUFFER_ALT[];
extern void FUN_8001ca58(void), updateAreaScreenEffect(void), FUN_8001d36c(void), updateObjectsUnlayered(void), FUN_800ec5e8(void);
extern void FUN_80039580(void);
extern void updateInventoryOverlay(void), FUN_800ebc30(void), updateObjectsLayer8(void), updateObjectsLayer7(void);
extern void resetDrawLists(void), FUN_8005042c(void), FUN_8001dbdc(void);

void areaMode4Tick(void)
{
    G164 = (unsigned int)(LOAD_BUFFER_ALT + G1f4 * 0xc000) & 0xffffff;
    FUN_8001ca58();
    if (G1c6 == 0) {
        G1f8 = G1f8 + 1;
        updateAreaScreenEffect();
        if (G1c6 == 0) {
            FUN_80039580();
            FUN_8001d36c();
            updateObjectsUnlayered();
            FUN_800ec5e8();
        }
    }
    if (G1c6 != 1)
        updateInventoryOverlay();
    if (G1c6 == 0) {
        updateObjectsLayer8();
        if (G1c6 == 0) {
            FUN_800ebc30();
            updateObjectsLayer7();
        }
    }
    if (G1c6 != 2)
        FUN_8005042c();
    else
        resetDrawLists();
    FUN_8001dbdc();
}
