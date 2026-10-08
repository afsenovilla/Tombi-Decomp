// FUNC 8005a9a4 188 MAIN0
// MATCHING 8005a9a4 188
extern unsigned char DAT_800805ec[];
extern unsigned char DAT_8009cda4[];
extern void FUN_80026a10(int a);
extern void FUN_8005aa74(int a, int b, int c, int d);
extern void FUN_8005b624(int a, int b);
extern void FUN_8001f2ec(int a);
extern void FUN_8001f620(void);

int FUN_8005a9a4(int i, int arg)
{
    int t;
    if (DAT_8009cda4[i] != 0xff) {
        t = DAT_800805ec[i];
        DAT_8009cda4[i] = 0xff;
        FUN_80026a10(((int *)(DAT_800805ec - 0xe8))[t]); /* tabla en 0x80080504: base simbolo+offset para que no se adelante al sb */
        if (i != 10) {
            FUN_8005aa74(i, 1, 1, arg);
            FUN_8005b624(i, 1);
            FUN_8001f2ec(2);
            FUN_8001f620();
        }
    }
    return DAT_8009cda4[i];
}
