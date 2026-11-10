// VERSION PCPX_96084 tomba 8001F2C0 236
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Movie mode steps: display off, display init (func_800172D8), text window, then the movie thread (func_800228B8 in slot 1);
// waits for D_800CCCDC to clear and switches to mode 2.
extern short *D_800DE0B0;
extern char D_800E5154[];
extern short D_800CCCDC, D_800DE084;
extern void SetDispMask(int);
extern void func_800172D8_pcpx96084(int, int, int);
extern void func_8001E084(char *);
extern void ThreadCreate(int, void (*)(void));
extern void func_800228B8(void);

void func_8001F2C0_pcpx96084(void)
{
    short *s = D_800DE0B0;
    unsigned short u = s[0x4a / 2];
    switch (u) {
    case 0:
        SetDispMask(0);
        func_800172D8_pcpx96084(0x6f, 0x7d, 0xd0);
        func_8001E084(D_800E5154);
        D_800CCCDC = 1;
        D_800DE084 = 0;
        D_800DE0B0[0x4a / 2]++;
        ThreadCreate(1, func_800228B8);
        break;
    case 1:
        if (D_800CCCDC == 0)
            s[0x4a / 2] = u + 1;
        break;
    case 2:
        s[0x48 / 2] = 2;
        s[0x4a / 2] = 0;
    }
}
#endif
