// FUNC 80025b98 92 MAIN0
// MATCHING 80025b98 92
extern char DAT_8009f7f0;
extern unsigned short DAT_1f8003da;
extern int PadInfoMode(int, int, int);
extern void PadSetMainMode(int, int, int);

void setAnalogPadMode(void)
{
    if (DAT_8009f7f0 == 0 && PadInfoMode(0, 2, 0) != 0 && DAT_1f8003da == 7)
        PadSetMainMode(0, 1, 0);
}
