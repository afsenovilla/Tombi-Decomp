// FUNC 800697b4 204 MAIN0
/* score 52: library code from a newer compiler (jr ra; addiu sp in delay slot with s0 saved, filled jal slots): not reproducible with CC1PSX 4.3. */
extern int DAT_800981e4;
extern int DAT_800981e0;
extern int DAT_8009bf78[];
extern int *DAT_8009820c;
extern void (*DAT_800981b0)(int);
extern int DAT_8009bf88, DAT_8009bf8c;
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern void SysDeqIntRP(int, int *);
extern void SysEnqIntRP(int, int *);
extern void ChangeClearRCnt(int, int);

void FUN_800697b4(void)
{
    int *p;
    DAT_800981e4 = 0;
    EnterCriticalSection();
    SysDeqIntRP(2, DAT_8009bf78);
    SysEnqIntRP(2, DAT_8009bf78);
    p = DAT_8009820c;
    p[0] = -2;
    p[1] = p[1] | 1;
    ChangeClearRCnt(3, 0);
    ExitCriticalSection();
    DAT_800981b0(DAT_800981e0);
    DAT_800981b0(DAT_800981e0 + 0xf0);
    DAT_8009bf8c = 0;
    DAT_8009bf88 = 0;
    DAT_800981e4 = 1;
}
