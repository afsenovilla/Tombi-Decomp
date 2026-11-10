// VERSION SLPM_80165 slpm80165 80018EC8 236
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_8001F2C0_pcpx96084 (src/func_8001F2C0_pcpx96084.c): movie mode steps, display off, display
// init (func_80015914_slpm80165 with 0, 0, 0; PrePre 8: 0x6F, 0x7D, 0xD0), text window, then the movie thread
// func_8001D1D8 in slot 1; waits for the scratchpad byte 0x1F8001CC to clear and switches to mode 3 (PrePre 8: 2).
extern short *D_1F8001D4;
extern unsigned char D_1F8001CC, D_1F8001CD;
extern char D_800A2A18[];
extern void SetDispMask(int);
extern void func_80015914_slpm80165(int, int, int);
extern void func_80018254(char *);
extern void ThreadCreate(int, void (*)(void));
extern void func_8001D1D8(void);

void func_80018EC8(void)
{
    short *s = D_1F8001D4;
    unsigned short u = s[0x4a / 2];
    switch (u) {
    case 0:
        SetDispMask(0);
        func_80015914_slpm80165(0, 0, 0);
        func_80018254(D_800A2A18);
        D_1F8001CC = 1;
        D_1F8001CD = 0;
        D_1F8001D4[0x4a / 2]++;
        ThreadCreate(1, func_8001D1D8);
        break;
    case 1:
        if (D_1F8001CC == 0)
            s[0x4a / 2] = u + 1;
        break;
    case 2:
        s[0x48 / 2] = 3;
        s[0x4a / 2] = 0;
    }
}
#endif
