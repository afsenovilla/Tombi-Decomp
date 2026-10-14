// FUNC 80025bf4 108 MAIN0
// MATCHING 80025bf4 108
extern short DAT_8009d612;
extern char DAT_8009d614[8];
extern char DAT_800a;
extern int DAT_8009f7f0[];
extern void PadInitDirect(int *, int *);
extern void PadStartCom(void);
void joypadInit(void)
{
DAT_8009d612 = 1;
DAT_8009d614[0] = 0;
DAT_8009d614[1] = 0;
DAT_8009d614[2] = 0;
DAT_8009d614[3] = 0;
DAT_8009d614[6] = 0;
DAT_8009d614[7] = 0;
PadInitDirect(DAT_8009f7f0, (int *)((char *)DAT_8009f7f0 + 0x22));
PadStartCom();
}
