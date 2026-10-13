// FUNC 8004f3bc 48 MAIN0
// MATCHING 8004f3bc 48
extern char LOAD_COMPLETE_SYM;
extern void ThreadCreate(int, void *);
extern char LAB_8004eb08[];

void startSoundTask(void)
{
    LOAD_COMPLETE_SYM = 0;
    ThreadCreate(2, LAB_8004eb08);
}
