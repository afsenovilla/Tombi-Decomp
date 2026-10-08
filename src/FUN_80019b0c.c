// FUNC 80019b0c 480 MAIN0
// MATCHING 80019b0c 480
typedef struct { char pad[0x48]; unsigned short w48, w4a, w4c, w4e; } S1d4;
extern S1d4 *DAT_1f8001d4;
extern unsigned char DAT_1f8001ce;
extern unsigned char DAT_1f8001d0;
extern short DAT_1f8001f4;
extern int DAT_1f800164;
extern short DAT_800a45ea;
extern unsigned char DAT_8009c974;
extern unsigned char DAT_8009c967;
extern char DAT_800b3e28[];
extern void FUN_8004dd8c(void);
extern void FUN_8005f060(int);
extern void FUN_800261b0(void);
extern void FUN_8004fb68(int);
extern void FUN_8004fa80(int, int);
extern void FUN_80018280(void);
extern void FUN_8001eff8(int);
extern int FUN_800e9888(void);
extern void FUN_800ea37c(void);
extern void FUN_8001dbdc(void);
extern void FUN_8001f110(int);
extern void FUN_80017328(void (*)(void));

void FUN_80019b0c(void)
{
    int r;
    switch (DAT_1f8001d4->w4a) {
    case 0:
        FUN_8005f060(0);
        FUN_800261b0();
        DAT_1f8001ce = 0;
        FUN_8004fb68(3);
        FUN_8004fa80(5, 1);
        DAT_1f8001d4->w4a++;
        break;
    case 1:
        if (DAT_1f8001ce == 0)
            break;
        FUN_80018280();
        DAT_800a45ea = 5;
        FUN_8001eff8(0);
        FUN_8005f060(1);
        DAT_1f8001d4->w4a++;
        break;
    case 2:
        DAT_1f800164 = (int)(DAT_800b3e28 + DAT_1f8001f4 * 0xc000) & 0xffffff;
        r = FUN_800e9888();
        if (r == 1)
            DAT_1f8001d4->w4a++;
        else if (r == -1)
            DAT_1f8001d4->w4a = 4;
        FUN_800ea37c();
        FUN_8001dbdc();
        break;
    case 3:
        DAT_8009c974 = 0;
        DAT_8009c967 = 0;
        FUN_8001f110(0);
        DAT_1f8001d4->w48 = 1;
        DAT_1f8001d4->w4a = 1;
        DAT_1f8001d4->w4c = 0;
        DAT_1f8001d4->w4e = 0;
        break;
    case 4:
        FUN_8001f110(0);
        DAT_1f8001d0 = 0;
        DAT_1f8001d4->w48 = 1;
        DAT_1f8001d4->w4a = 0;
        DAT_1f8001d4->w4c = 0;
        DAT_1f8001d4->w4e = 0;
        FUN_80017328(FUN_8004dd8c);
        break;
    }
}
