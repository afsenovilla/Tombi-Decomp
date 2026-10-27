// FUNC 800efd10 92 D0SPSUB
// MATCHING 800efd10 92
extern unsigned char SUBSCREEN_EVENT_ORDER[];
extern unsigned char GAME_INVENTORY_SCREEN[];

int subScreenCountEvents(void)
{
    int last = 0;
    int i;
    for (i = 0; i < 0x100; i++) {
        if (SUBSCREEN_EVENT_ORDER[i] != 0xFF && GAME_INVENTORY_SCREEN[SUBSCREEN_EVENT_ORDER[i]] != 0) last = i;
    }
    return last + 1;
}
