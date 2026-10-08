// FUNC 8001f850 148 MAIN0
// MATCHING 8001f850 148
extern volatile unsigned short DAT_8009d670[];
extern volatile unsigned short DAT_8009d672;
extern volatile unsigned short DAT_8009d674;
extern volatile unsigned short DAT_8009d676;
extern unsigned short DAT_1f8001fc;
extern unsigned short DAT_1f8001fe;
extern unsigned short FUN_8002623c(int);
extern void FUN_80026000(void);

void FUN_8001f850(void)
{
    DAT_8009d674 = DAT_8009d670[0];
    DAT_8009d676 = DAT_8009d672;
    DAT_8009d670[0] = FUN_8002623c(0);
    DAT_1f8001fc = ~DAT_8009d674 & DAT_8009d670[0];
    DAT_1f8001fe = ~DAT_8009d670[0] & DAT_8009d674;
    FUN_80026000();
}
