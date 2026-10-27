// FUNC 800ee6b4 260 D0SPSUB
// MATCHING 800ee6b4 260
// FLAGS -O2 -G0 -fno-schedule-insns
extern unsigned short D_8009F7F2;
extern short SUBSCREEN_MAP_VIEW_X, SUBSCREEN_MAP_VIEW_Y;
extern unsigned short JOYPAD_STATE;

int subScreenMapReadInput(void)
{
    int pad = ~D_8009F7F2;
    if (pad & 0xC00) {
        if (pad & 0x10) {
            SUBSCREEN_MAP_VIEW_X += 2;
            return -1;
        }
        if (pad & 0x40) {
            SUBSCREEN_MAP_VIEW_X -= 2;
            return -1;
        }
        if (pad & 0x80) {
            SUBSCREEN_MAP_VIEW_Y += 2;
            return -1;
        }
        if (pad & 0x20) {
            SUBSCREEN_MAP_VIEW_Y -= 2;
            return -1;
        }
    } else {
        if (JOYPAD_STATE & 0x80) return 4;
        if (JOYPAD_STATE & 0x20) return 0;
        if (JOYPAD_STATE & 0x10) return 2;
        if (JOYPAD_STATE & 0x40) return 6;
    }
    return -1;
}
