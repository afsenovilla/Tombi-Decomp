// FUNC 8001f850 148 MAIN0
extern volatile unsigned short DAT_8009d670[];
extern unsigned short SP[];
extern volatile unsigned short SV[];
extern unsigned short FUN_8002623c(int);
extern void FUN_80026000(void);

void FUN_8001f850(void)
{
    DAT_8009d670[2] = DAT_8009d670[0];
    DAT_8009d670[3] = DAT_8009d670[1];
    DAT_8009d670[0] = FUN_8002623c(0);
    SV[0xfe] = DAT_8009d670[0] & ~DAT_8009d670[2];
    SV[0xff] = DAT_8009d670[2] & ~DAT_8009d670[0];
    FUN_80026000();
}
