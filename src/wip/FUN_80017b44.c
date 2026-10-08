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
    unsigned int n, q7, q6, q5, q4, q3, q2, q1;
    char *p;
    unsigned int d0, d1, d2, d3, d4, d5, d6, d7;

    memset(DAT_8009c930, 0, 0x2c);
    DAT_1f8001c6 = 0;
    DAT_8009c975 = 1;
    DAT_1f8003d0 = 0;
    DAT_8009d00f = 0;
    FUN_80018f68();
    FUN_8001f1c0();
    n = DAT_8009c96c;
    q7 = n / 10000000;
    q6 = n / 1000000;
    q5 = n / 100000;
    q4 = n / 10000;
    q3 = n / 1000;
    q2 = n / 100;
    q1 = n / 10;
    d0 = q7 % 10;
    d1 = q6 - q7 * 10;
    d2 = q5 - q6 * 10;
    d3 = q4 - q5 * 10;
    d4 = q3 - q4 * 10;
    d5 = q2 - q3 * 10;
    d6 = q1 - q2 * 10;
    d7 = n - q1 * 10;
    p = DAT_800b144c;
    *p++ = d0;
    *p++ = d1;
    *p++ = d2;
    *p++ = d3;
    *p++ = d4;
    *p++ = d5;
    *p++ = d6;
    *p = d7;
}
