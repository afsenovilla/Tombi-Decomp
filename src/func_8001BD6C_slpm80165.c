// VERSION SLPM_80165 slpm80165 8001BD6C 200
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_80021238 (src/func_80021238_pcpx96084.c): mode step, starts func_80018254 on the text
// window D_800A2A18, waits one frame, then switches the task to mode 3. The task pointer is the scratchpad word
// 0x1F8001D4 (PrePre 8: RAM D_800DE0B0).
extern short *D_1F8001D4;
extern char D_800A2A18[];
extern void func_80018254(char *);

void func_8001BD6C(void)
{
    short *s = D_1F8001D4;
    switch ((unsigned short)s[0x4c / 2]) {
    case 0:
        func_80018254(D_800A2A18);
        D_1F8001D4[0x5c / 2] = 1;
        D_1F8001D4[0x4c / 2]++;
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
