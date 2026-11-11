// VERSION PCPX_96084 tomba 80021238 200
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Mode step: starts func_8001E084 on D_800E5154, waits one frame, then switches the task to mode 3.
extern short *D_800DE0B0;
extern char D_800E5154[];
extern void func_8001E084(char *);

void func_80021238(void)
{
    short *s = D_800DE0B0;
    switch ((unsigned short)s[0x4c / 2]) {
    case 0:
        func_8001E084(D_800E5154);
        D_800DE0B0[0x5c / 2] = 1;
        D_800DE0B0[0x4c / 2]++;
        break;
    case 1:
        if (--s[0x5c / 2] <= 0)
            s[0x4c / 2]++;
        break;
    case 2:
        s[0x4a / 2] = 3;
        s[0x4c / 2] = 0;
    }
}
#endif
