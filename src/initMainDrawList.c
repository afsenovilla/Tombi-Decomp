// FUNC 80017f80 108 MAIN0
// MATCHING 80017f80 108
extern char DAT_800b1320[];
extern char *DAT_1f800220, *DAT_1f800264;
extern short CAT3_OBJECT_COUNT_SAVED, CAT3_OBJECT_COUNT;
extern char LOAD_BUFFER[];
extern void initSpriteSlots(void);

void initMainDrawList(void)
{
    int i = 7;
    int m = -1;
    int off = 0x3d4;
    DAT_1f800220 = DAT_800b1320;
    DAT_1f800264 = DAT_800b1320;
    CAT3_OBJECT_COUNT_SAVED = 0;
    CAT3_OBJECT_COUNT = 0;
    do {
        *(short *)(LOAD_BUFFER + off) = m;
        i--;
        off -= 0x8c;
    } while (i >= 0);
    initSpriteSlots();
}
