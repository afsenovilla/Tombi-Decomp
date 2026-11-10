// VERSION PCPX_96084 tomba 8001FACC 244
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Message mode: prints the string D_80010180 at (8, 12) in the text window, waits 120 frames, then mode 0.
extern short *D_800DE0B0;
extern char D_800E5154[];
extern char D_80010180[];
extern void func_8001E084(char *);
extern void func_8001EA60_pcpx96084(char *, int, int, int);
extern void func_8001E2D4(char *, char *);

void func_8001FACC_pcpx96084(void)
{
    short *s = D_800DE0B0;
    switch ((unsigned short)s[0x4a / 2]) {
    case 0:
        func_8001E084(D_800E5154);
        func_8001EA60_pcpx96084(D_800E5154, 8, 12, 0);
        func_8001E2D4(D_800E5154, D_80010180);
        D_800DE0B0[0x58 / 2] = 0x78;
        D_800DE0B0[0x4a / 2]++;
        break;
    case 1:
        if (--s[0x58 / 2] <= 0)
            s[0x4a / 2]++;
        break;
    case 2:
        s[0x48 / 2] = 0;
        s[0x4a / 2] = 0;
    }
}
#endif
