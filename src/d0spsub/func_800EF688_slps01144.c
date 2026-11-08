// VERSION SLPS_01144 dspsub 800EF688 308
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ee6b4 (subScreenMapReadInput.c): held pad word read directly
extern unsigned short D_800A1380;
extern short D_801113BC, D_801113C4;
extern unsigned short JOYPAD_STATE;

int func_800EF688(void)
{
    volatile unsigned short *pad = &D_800A1380;
    if (*pad & 0xC) {
        if (*pad & 0x1000) D_801113BC += 2;
        else if (*pad & 0x4000) D_801113BC -= 2;
        else if (*pad & 0x8000) D_801113C4 += 2;
        else if (*pad & 0x2000) D_801113C4 -= 2;
    } else {
        if (JOYPAD_STATE & 0x8000) return 4;
        if (JOYPAD_STATE & 0x2000) return 0;
        if (JOYPAD_STATE & 0x1000) return 2;
        if (JOYPAD_STATE & 0x4000) return 6;
    }
    return -1;
}
#endif
