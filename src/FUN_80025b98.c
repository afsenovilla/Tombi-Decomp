// FUNC 80025b98 92 MAIN0
// MATCHING 80025b98 92
extern char DAT_8009f7f0;
extern unsigned short DAT_1f8003da;
extern int FUN_8006911c(int, int, int);
extern void FUN_800693c8(int, int, int);

void FUN_80025b98(void)
{
    if (DAT_8009f7f0 == 0 && FUN_8006911c(0, 2, 0) != 0 && DAT_1f8003da == 7)
        FUN_800693c8(0, 1, 0);
}
