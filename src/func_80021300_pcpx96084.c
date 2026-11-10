// VERSION PCPX_96084 tomba 80021300 300
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Game over / end mode: prints D_80010248 at (12, 12), waits 80 frames, then returns to the front-end task
// (func_8001ECD4_pcpx96084) in mode 2.
extern short *D_800DE0B0;
extern char D_800E5154[];
extern char D_80010248[];
extern int D_800E43D4;
extern unsigned char D_800E2B28;
extern void func_8001E084(char *);
extern void func_8001EA60_pcpx96084(char *, int, int, int);
extern void func_8001E2D4(char *, char *);
extern void func_80023514_pcpx96084(void), func_80022F34_pcpx96084(void);
extern void func_80017988_pcpx96084(void (*)(void));
extern void func_8001ECD4_pcpx96084(void);

void func_80021300(void)
{
    short *s = D_800DE0B0;
    switch ((unsigned short)s[0x4c / 2]) {
    case 0:
        D_800E43D4 = 0;
        func_8001E084(D_800E5154);
        func_8001EA60_pcpx96084(D_800E5154, 12, 12, 0);
        func_8001E2D4(D_800E5154, D_80010248);
        func_80023514_pcpx96084();
        D_800DE0B0[0x5c / 2] = 0x50;
        D_800DE0B0[0x4c / 2]++;
        func_80022F34_pcpx96084();
        break;
    case 1:
        if (--s[0x5c / 2] <= 0)
            s[0x4c / 2]++;
        break;
    case 2:
        s[0x48 / 2] = 2;
        s[0x4a / 2] = 0;
        s[0x4c / 2] = 0;
        s[0x4e / 2] = 0;
        D_800E2B28 = 0;
        func_80017988_pcpx96084(func_8001ECD4_pcpx96084);
    }
}
#endif
