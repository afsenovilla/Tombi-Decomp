// FUNC 800198b4 340 MAIN0
// MATCHING 800198b4 340
extern unsigned char D_1F800118[];
extern unsigned char D_8009E375, D_8009E376, D_8009E377;
extern unsigned char D_8009F085, D_8009F086, D_8009F087;
extern int D_8009F7E4;
extern short D_8009F838;
void FUN_80021fac(void *p);
void FUN_8001f850(void);
void FUN_80019a08(void);
void FUN_80019cec(void);
void FUN_80019b0c(void);
void ThreadWaitFrames(int n);
void func_800198B4(void)
{
    unsigned char *p = *(unsigned char **)0x1F8001D4;
    *(unsigned char *)0x1F8001D1 = 0;
    *(unsigned char *)0x1F8001D0 = 1;
    *(short *)(p + 0x48) = 0;
    *(short *)(p + 0x4a) = 0;
    *(short *)(p + 0x4c) = 0;
    *(short *)(p + 0x4e) = 0;
    p[0x6a] = 0;
    D_8009E375 = 0;
    D_8009E376 = 0;
    D_8009E377 = 0;
    D_8009F085 = 0;
    D_8009F086 = 0;
    D_8009F087 = 0;
    *(short *)0x1F8001DC = -1;
    *(short *)0x1F8001DE = 0;
    D_8009F7E4 = 0;
    *(unsigned char *)0x1F8001CE = 0;
    D_8009F838 = 0;
    FUN_80021fac(D_1F800118);
    *(short *)0x1F8001FC = 0;
    for (;;) {
        FUN_8001f850();
        switch (*(unsigned short *)(*(unsigned char **)0x1F8001D4 + 0x48)) {
        case 0: FUN_80019a08(); break;
        case 1: FUN_80019cec(); break;
        case 2: FUN_80019b0c(); break;
        }
        ThreadWaitFrames(1);
    }
}
