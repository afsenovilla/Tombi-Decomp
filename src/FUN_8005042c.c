// FUNC 8005042c 280 MAIN0
// MATCHING 8005042c 280
extern void FUN_80063e0c(void *);
extern void FUN_80050544(void), FUN_80050ea4(void), FUN_80054f54(void), FUN_80055174(void);
extern void FUN_80057288(void), FUN_800571a8(void), FUN_80057d04(void), FUN_80057dfc(void);
extern void FUN_80057edc(void), FUN_800597bc(void), FUN_8011bc34(void), FUN_80058604(void);
extern void FUN_80059d44(int);
extern char DAT_1f800118[];
extern short DAT_1f8001c6;
extern unsigned char DAT_8009c975, DAT_8009c976;
extern int DAT_8009c960;

void FUN_8005042c(void)
{
    int u;
    FUN_80063e0c(DAT_1f800118);
    FUN_80050544();
    FUN_80050ea4();
    FUN_80054f54();
    FUN_80055174();
    FUN_80057288();
    FUN_800571a8();
    FUN_80057d04();
    FUN_80057dfc();
    FUN_80057edc();
    if (DAT_1f8001c6 == 1)
        FUN_800597bc();
    switch (DAT_8009c975) {
    case 1:
    case 4:
        u = 0xff;
        break;
    case 2:
        u = 0xff;
        u -= DAT_8009c976;
        break;
    case 3:
    case 0x10:
        u = DAT_8009c976;
        break;
    default:
        goto end;
    }
    FUN_80059d44(u);
end:
    if (DAT_8009c960 == 6)
        FUN_8011bc34();
    else
        FUN_80058604();
}
