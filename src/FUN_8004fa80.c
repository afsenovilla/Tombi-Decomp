// FUNC 8004fa80 232 MAIN0
// MATCHING 8004fa80 232
extern unsigned char DAT_1f8003d2;
extern unsigned char DAT_1f8001ce;
extern int DAT_8007c5f8;
extern int DAT_8009a930[];
extern unsigned short DAT_801fd8e0;
extern void FUN_8004f538(int);
extern void FUN_8004eb08(void);
extern void ThreadCreate(int, void (*)(void));
extern void ThreadWaitFrames(int);
void FUN_8004fa80(int a, int b)
{
    if (DAT_1f8003d2 != 0) {
        DAT_1f8003d2 = 0;
        FUN_8004f538(DAT_8007c5f8);
        DAT_1f8001ce = 0;
        ThreadCreate(2, FUN_8004eb08);
        while (DAT_1f8001ce == 0 && DAT_801fd8e0 != 0)
            ThreadWaitFrames(1);
    }
    FUN_8004f538(DAT_8009a930[a]);
    if (b != 0) {
        DAT_1f8001ce = 0;
        ThreadCreate(2, FUN_8004eb08);
    }
}
