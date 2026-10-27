// FUNC 800efd10 92 D0SPSUB
// MATCHING 800efd10 92
extern unsigned char D_801086F0[];
extern unsigned char GAME_INVENTORY_SCREEN[];

int subScreenCountEvents(void)
{
    int last = 0;
    int i;
    for (i = 0; i < 0x100; i++) {
        if (D_801086F0[i] != 0xFF && GAME_INVENTORY_SCREEN[D_801086F0[i]] != 0) last = i;
    }
    return last + 1;
}
