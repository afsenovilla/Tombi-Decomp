// VERSION SLPM_80165 slpm80165 80018BBC 464
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of titleSequenceTask (src/titleSequenceTask.c): the front-end task loop, seven modes (2 is the
// movie mode func_80018EC8); Start in any mode but 0 and 3 jumps to mode 3 (and flags a playing movie). Each
// frame also sets the primitive pointer and draws the text window D_800A2A18.
typedef struct { char p0[0x48]; unsigned short w48, w4a, w4c; char p4e[0x69 - 0x4e]; unsigned char b69; } Task;
extern Task *D_1F8001D4;
extern unsigned char D_1F8001D1, D_1F8001D0, D_1F8001CC, D_1F8001D3;
extern unsigned short D_1F8001F8, D_1F8001FC;
extern short D_1F8001F4;
extern int D_1F800164;
extern int D_1F8001E0;
extern char D_800B3498[];
extern char D_800A2A18[];
extern void func_8001FF34(void);
extern void func_80019384(void), func_80018D8C(unsigned int *), func_80018EC8(void), func_80018FB4(unsigned int *);
extern void func_8001936C(void), func_80019374(void), func_8001937C(void);
extern void func_800184B4(int, char *);
extern void ThreadWaitFrames(int);

void func_80018BBC(void)
{
    Task *t = D_1F8001D4;
    unsigned int sp10[2];
    unsigned short s;

    D_1F8001D1 = 1;
    D_1F8001D0 = 0;
    sp10[0] = 0;
    t->w4a = 0;
    t->b69 = 0;
    while (1) {
        D_1F8001F8 = D_1F8001F8 + 1;
        D_1F800164 = (int)(D_800B3498 + D_1F8001F4 * 0xc000) & 0xffffff;
        func_8001FF34();
        t = D_1F8001D4;
        s = t->w48;
        if (s != 0 && (D_1F8001FC & 0x800) && s != 3) {
            t->w48 = 3;
            t->w4a = 0;
            t->w4c = 0;
            sp10[0] = 1;
            if (D_1F8001CC)
                D_1F8001D3 = 1;
        }
        switch (D_1F8001D4->w48) {
        case 0:
            func_80019384();
            break;
        case 1:
            func_80018D8C(sp10);
            break;
        case 2:
            func_80018EC8();
            break;
        case 3:
            func_80018FB4(sp10);
            break;
        case 4:
            func_8001936C();
            break;
        case 5:
            func_80019374();
            break;
        case 6:
            func_8001937C();
            break;
        }
        func_800184B4(D_1F8001E0, D_800A2A18);
        ThreadWaitFrames(1);
    }
}
#endif
