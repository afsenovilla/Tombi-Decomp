// FUNC 8005a7a0 264 MAIN0
// MATCHING 8005a7a0 264
extern int DAT_80080504[];
extern unsigned char DAT_80080524[];
extern unsigned char DAT_800805ec[];
extern unsigned char DAT_8009cda4[];
extern void FUN_80026a10(int);
extern void FUN_8005aa74(int, int, int, int);
extern void FUN_8005b624(int, int);
extern void FUN_8001e4f0(int);
extern void FUN_8002b8cc(int);
extern void FUN_8001f2ec(int);
extern void FUN_8001f620(void);

unsigned char FUN_8005a7a0(int a, int b, int c)
{
    if (b == 0) {
        FUN_80026a10(DAT_80080504[DAT_80080524[a]]);
        if (a != 10) {
            FUN_8005aa74(a, 0, 0x3c, c);
            FUN_8005b624(a, 0);
            FUN_8001e4f0(0x2a);
            FUN_8002b8cc(0);
        }
    } else {
        FUN_80026a10(DAT_80080504[DAT_800805ec[a]]);
        if (a != 10) {
            FUN_8005aa74(a, 1, 1, c);
            FUN_8005b624(a, 1);
            FUN_8001f2ec(2);
            FUN_8001f620();
        }
    }
    return DAT_8009cda4[a];
}
