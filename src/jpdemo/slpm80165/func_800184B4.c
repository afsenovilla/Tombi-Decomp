// JPDEMO slpm80165 800184B4 56
// Demo-only helper: runs func_800184EC unless both scratchpad flags 0x1CF/0x1D0 are set.
extern unsigned char D_1F8001CF, D_1F8001D0;
extern void func_800184EC(void);
void func_800184B4(void)
{
    if ((D_1F8001CF & D_1F8001D0) == 0)
        func_800184EC();
}
