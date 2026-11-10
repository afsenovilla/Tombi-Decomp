// VERSION SLPM_80165 slpm80165 8001BE34 292
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_80021300 (src/func_80021300_pcpx96084.c): end mode, prints D_80010284 at (12, 12), waits
// 90 frames (PrePre 8: 80), then returns to the front-end task func_80018BBC in mode 1 (PrePre 8: 2). Task
// pointer and the cleared byte are scratchpad words here.
extern short *D_1F8001D4;
extern unsigned char D_1F8001D0;
extern char D_800A2A18[];
extern char D_80010284[];
extern void func_80018254(char *);
extern void func_80018638(char *, int, int, int);
extern void func_8001836C(char *, char *);
extern void func_8001EA1C(int), func_8001EB30(void);
extern void setTask(void (*)(void));
extern void func_80018BBC(void);

void func_8001BE34(void)
{
    short *s = D_1F8001D4;
    switch ((unsigned short)s[0x4c / 2]) {
    case 0:
        func_80018254(D_800A2A18);
        func_80018638(D_800A2A18, 12, 12, 0);
        func_8001836C(D_800A2A18, D_80010284);
        func_8001EA1C(0);
        D_1F8001D4[0x5c / 2] = 0x5a;
        D_1F8001D4[0x4c / 2]++;
        func_8001EB30();
        break;
    case 1:
        if (--s[0x5c / 2] <= 0)
            s[0x4c / 2]++;
        break;
    case 2:
        s[0x48 / 2] = 1;
        s[0x4a / 2] = 0;
        s[0x4c / 2] = 0;
        s[0x4e / 2] = 0;
        D_1F8001D0 = 0;
        setTask(func_80018BBC);
    }
}
#endif
