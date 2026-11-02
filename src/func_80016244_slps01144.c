// near-twin of PAL initDisplay (tools/jp_autofix.py)
// VERSION SLPS_01144 slps01144 80016244 388
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
/* NTSC twin of initDisplay (JP 800163C8 is the 320x240 one): 320x224 display, screen offset y 8. */
extern void FUN_8005e244(void *, int, int, int, int);
extern void FUN_8005e314(void *, int, int, int, int);
extern void FUN_8005f9c8(void *);
extern void FUN_8005f770(void *);
extern char DAT_8009e35c[];
extern short D_8009FEE0, D_8009FEE2, D_8009FEE4, D_8009FEE6, D_800A0BF0, D_800A0BF2, D_800A0BF4, D_800A0BF6;
extern char D_800A0C12, D_800A0C13, D_800A0C14, D_8009FF02, D_8009FF03, D_8009FF04;
extern char DISPLAY_BUFFER_0_BG_R, DISPLAY_BUFFER_0_BG_G, DISPLAY_BUFFER_0_BG_B, DISPLAY_BUFFER_1_BG_R, DISPLAY_BUFFER_1_BG_G, DISPLAY_BUFFER_1_BG_B;

void func_80016244(char a, char b, char c)
{
    FUN_8005e244(DAT_8009e35c, 0x180, 0x100, 0x140, 0xe0);
    FUN_8005e314(DAT_8009e35c - 0x14, 0x2c0, 0x100, 0x140, 0xe0);
    FUN_8005e244(DAT_8009e35c + 0xd10, 0x2c0, 0x100, 0x140, 0xe0);
    FUN_8005e314(DAT_8009e35c + 0xcfc, 0x180, 0x100, 0x140, 0xe0);
    D_8009FEE0 = 0;
    D_8009FEE2 = 8;
    D_8009FEE4 = 0x100;
    D_8009FEE6 = 0xe0;
    D_800A0BF6 = 0xe0;
    D_800A0BF0 = 0;
    D_800A0BF2 = 8;
    D_800A0BF4 = 0x100;
    D_800A0C14 = 1;
    D_8009FF04 = 1;
    D_800A0C12 = 1;
    D_8009FF02 = 1;
    D_800A0C13 = 0;
    D_8009FF03 = 0;
    DISPLAY_BUFFER_0_BG_R = a;
    DISPLAY_BUFFER_0_BG_G = b;
    DISPLAY_BUFFER_0_BG_B = c;
    DISPLAY_BUFFER_1_BG_R = a;
    DISPLAY_BUFFER_1_BG_G = b;
    DISPLAY_BUFFER_1_BG_B = c;
    FUN_8005f9c8(DAT_8009e35c - 0x14);
    FUN_8005f770(DAT_8009e35c);
}
#endif
