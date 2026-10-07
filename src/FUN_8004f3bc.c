// FUNC 8004f3bc 48 MAIN0
// MATCHING 8004f3bc 48
extern char DAT_1f8001ce;
extern void ThreadCreate(int, void *);
extern char LAB_8004eb08[];

void FUN_8004f3bc(void)
{
    DAT_1f8001ce = 0;
    ThreadCreate(2, LAB_8004eb08);
}
