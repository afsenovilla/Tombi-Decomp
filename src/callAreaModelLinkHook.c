// FUNC 8002af10 264 MAIN0
// MATCHING 8002af10 264
extern unsigned short GAME;
extern unsigned short GAME_SELECTED_SECTION;
extern unsigned char DAT_800b0d9b;
extern unsigned char DAT_800b0d99;
extern unsigned char GAME_DISABLE_SELECT_MENU;
extern char DAT_800b0d98[];
extern int WMD_DATA;
extern void (*PTR_DAT_80079b3c[])(char *, int);

void callAreaModelLinkHook(void)
{
    unsigned short v, x;
    DAT_800b0d9b = 0;
    switch (GAME) {
    case 0:
        break;
    case 0xd:
        if (GAME_SELECTED_SECTION != 0) return;
        break;
    case 2:
        x = GAME_SELECTED_SECTION;
        v = 3;
        goto cmp;
    case 6:
        if (GAME_SELECTED_SECTION > 1) return;
        break;
    case 9:
        x = GAME_SELECTED_SECTION;
        if (x == 0) break;
        v = 6;
        goto cmp;
    case 0x12:
        x = GAME_SELECTED_SECTION;
        v = 1;
    cmp:
        if (x != v) return;
        break;
    case 5: case 8: case 0xb: case 0x10: case 0x11: case 0x13:
        return;
    }
    PTR_DAT_80079b3c[GAME](DAT_800b0d98, WMD_DATA);
    DAT_800b0d99 = GAME_DISABLE_SELECT_MENU;
}
