// FUNC 8004f3ec 164 MAIN0
#define DAT_1f8003d2 (*(unsigned char *)(0x1f800000 + 0x3d2))
#define DAT_1f8001ce (*(unsigned char *)(0x1f800000+0x1ce))
extern unsigned short DAT_801fd8e0;
extern int PTR_DAT_8007c5fc[];
extern void FUN_8004f538(int);
extern void ThreadCreate(int, void *);
extern void ThreadWaitFrames(int);
extern char LAB_8004eb08[];

void FUN_8004f3ec(unsigned a)
{
    unsigned t = a + 1;
    if (DAT_1f8003d2 != t) {
        { int v = PTR_DAT_8007c5fc[a]; DAT_1f8003d2 = t; FUN_8004f538(v); }
        DAT_1f8001ce = 0;
        ThreadCreate(2, LAB_8004eb08);
        if (DAT_1f8001ce == 0)
            while (DAT_801fd8e0 != 0) {
                ThreadWaitFrames(1);
                if (DAT_1f8001ce != 0) break;
            }
    }
}
