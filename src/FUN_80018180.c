// FUNC 80018180 212 MAIN0
// MATCHING 80018180 212
extern unsigned char DAT_800a49a8[];
extern unsigned char DAT_800a4d74[];
extern int DAT_800a4468[];
extern char DAT_800a64f8[];
extern int *DAT_1f800214;
extern short DAT_1f80023c, DAT_1f80025a, DAT_1f800240;
extern char *DAT_1f80022c, *DAT_1f800270;
extern void memset_(void *, int, int);
void FUN_80018180(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = DAT_800a49a8;
    for (; i < 10; i++) {
        memset_(p, 0, 0x6c);
        p += 0x6c;
    }
    q = DAT_800a4d74;
    DAT_1f800214 = DAT_800a4468;
    for (i = 0; i < 10; i++) {
        q[0x1c] = 7;
        *--DAT_1f800214 = (int)q;
        q -= 0x6c;
    }
    DAT_1f80023c = 10;
    DAT_1f80022c = DAT_800a64f8;
    DAT_1f800270 = DAT_800a64f8;
    DAT_1f80025a = 0;
    DAT_1f800240 = 0;
}
