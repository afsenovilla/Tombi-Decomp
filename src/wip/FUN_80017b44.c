// FUNC 80017b44 448 MAIN0
extern void memset(void *, int, int);
extern void FUN_80018f68(void);
extern void FUN_8001f1c0(void);
extern char DAT_8009c930[];
extern short DAT_1f8001c6;
extern char DAT_8009c975;
extern char DAT_1f8003d0;
extern char DAT_8009d00f;
extern unsigned int DAT_8009c96c;
extern char DAT_800b144c[];

void FUN_80017b44(void)
{
    unsigned int n;
    char *p;

    memset(DAT_8009c930, 0, 0x2c);
    DAT_1f8001c6 = 0;
    DAT_8009c975 = 1;
    DAT_1f8003d0 = 0;
    DAT_8009d00f = 0;
    FUN_80018f68();
    FUN_8001f1c0();
    n = DAT_8009c96c;
    p = DAT_800b144c;
    *p++ = n / 10000000 % 10;
    *p++ = n / 1000000 % 10;
    *p++ = n / 100000 % 10;
    *p++ = n / 10000 % 10;
    *p++ = n / 1000 % 10;
    *p++ = n / 100 % 10;
    *p++ = n / 10 % 10;
    *p = n % 10;
}
