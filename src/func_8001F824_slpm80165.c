// VERSION SLPM_80165 slpm80165 8001F824 100
#if defined(VERSION_SLPM_80165)
// Demo-only helper: queues load list [i][j] and starts task 2 if it is idle.
extern int *D_80070A44[];
extern unsigned short D_801FD8E0;
extern unsigned char D_1F8001CE;
extern void func_8001F968(int);
extern void func_8001EE10(void);
extern void ThreadCreate(int, void (*)(void));
void func_8001F824_slpm80165(int i, int j)
{
    func_8001F968(D_80070A44[i][j]);
    D_1F8001CE = 0;
    if (D_801FD8E0 == 0)
        ThreadCreate(2, func_8001EE10);
}
#endif
