// FUNC 80025bf4 108 MAIN0
// MATCHING 80025bf4 108
extern short DAT_8009d612;
extern char DAT_8009d614[8];
extern char DAT_800a;
extern int DAT_8009f7f0[];
extern void FUN_8006b464(int *, int *);
extern void FUN_80068fc4(void);
void FUN_80025bf4(void)
{
DAT_8009d612 = 1;
DAT_8009d614[0] = 0;
DAT_8009d614[1] = 0;
DAT_8009d614[2] = 0;
DAT_8009d614[3] = 0;
DAT_8009d614[6] = 0;
DAT_8009d614[7] = 0;
FUN_8006b464(DAT_8009f7f0, (int *)((char *)DAT_8009f7f0 + 0x22));
FUN_80068fc4();
}
