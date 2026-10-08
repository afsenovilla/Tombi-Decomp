// FUNC 8005a8a8 252 MAIN0
// MATCHING 8005a8a8 252
extern unsigned char DAT_8009cda4[];
extern unsigned char DAT_8009cda5;
extern int DAT_8009c960;
extern unsigned char DAT_80080524[];
extern int DAT_80080504[];
extern void FUN_80026a10(int);
extern void FUN_8005aa74(int, int, int, int);
extern void FUN_8005b624(int, int);
extern void SfxPlay(int);
extern void FUN_8002b8cc(int);

unsigned char FUN_8005a8a8(int p, int b, int c)
{
    if (DAT_8009cda4[p] == 0) {
        if (p == 1) {
            if (DAT_8009c960 == 0) DAT_8009cda5++;
        } else {
            DAT_8009cda4[p]++;
        }
        FUN_80026a10(DAT_80080504[DAT_80080524[p]]);
        if (p != 10) {
            FUN_8005aa74(p, 0, 0x3c, c);
            FUN_8005b624(p, 0);
            SfxPlay(0x2a);
            FUN_8002b8cc(0);
        }
    }
    return DAT_8009cda4[p];
}
