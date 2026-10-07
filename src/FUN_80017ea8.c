// FUNC 80017ea8 168 MAIN0
// MATCHING 80017ea8 168
extern char DAT_800a6610[];
extern char DAT_800b0adc[];
extern char DAT_800a49a8[];
extern char *DAT_1f800208;
extern short DAT_1f800238;
extern void memset(void *, int, int);

void FUN_80017ea8(void)
{
    int i;
    char *p;
    char *r;
    char **q;
    i = 0;
    p = DAT_800a6610;
    do {
        memset(p, 0, 0xd4);
        i++;
        p += 0xd4;
    } while (i < 200);
    r = DAT_800b0adc;
    DAT_1f800208 = DAT_800a49a8;
    i = 0;
    do {
        r[0x1c] = 0;
        i++;
        q = (char **)(DAT_1f800208 - 4);
        DAT_1f800208 = DAT_1f800208 - 4;
        *q = r;
        r -= 0xd4;
    } while (i < 200);
    DAT_1f800238 = 200;
}
