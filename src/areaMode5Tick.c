// FUNC 8001b9b0 324 MAIN0
// MATCHING 8001b9b0 324
extern char LOAD_BUFFER_ALT[];
extern short DAT_1f8001f4;
extern short DAT_1f8001c6;
extern unsigned short DAT_1f8001f8;
extern unsigned int DAT_1f800164;
extern void FUN_8001ca58(void);
extern void updateAreaScreenEffect(void);
extern void FUN_8001d36c(void);
extern void updateObjectsUnlayered(void);
extern void FUN_800edeec(void);
extern void updateInventoryOverlay(void);
extern void updateObjectsLayer8(void);
extern void FUN_800ed4d0(void);
extern void updateObjectsLayer7(void);
extern void resetDrawLists(void);
extern void FUN_8005042c(void);
extern void SoundUpdate(void);

void areaMode5Tick(void)
{
    DAT_1f800164 = (unsigned int)(LOAD_BUFFER_ALT + DAT_1f8001f4 * 0xc000) & 0xffffff;
    FUN_8001ca58();
    if (DAT_1f8001c6 == 0) {
        DAT_1f8001f8 = DAT_1f8001f8 + 1;
        updateAreaScreenEffect();
        if (DAT_1f8001c6 == 0) {
            FUN_8001d36c();
            updateObjectsUnlayered();
            FUN_800edeec();
        }
    }
    if (DAT_1f8001c6 != 1)
        updateInventoryOverlay();
    if (DAT_1f8001c6 == 0) {
        updateObjectsLayer8();
        if (DAT_1f8001c6 == 0) {
            FUN_800ed4d0();
            updateObjectsLayer7();
        }
    }
    if (DAT_1f8001c6 != 2)
        FUN_8005042c();
    else
        resetDrawLists();
    SoundUpdate();
}
