// FUNC 80017d30 164 MAIN0
// MATCHING 80017d30 164
extern unsigned char DAT_800a6268[];
extern unsigned char DAT_800a6484[];
extern unsigned char DAT_800b11b8[];
extern unsigned char **DAT_1f800210;
extern short DAT_1f80023e;
extern void *memset(void *, int, int);

void FUN_80017d30(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = DAT_800a6268;
    do {
        memset(p, 0, 0x3c);
        i++;
        p += 0x3c;
    } while (i < 10);
    q = DAT_800a6484;
    DAT_1f800210 = (unsigned char **)DAT_800b11b8;
    i = 0;
    do {
        i++;
        *--DAT_1f800210 = q;
        q -= 0x3c;
    } while (i < 10);
    DAT_1f80023e = 10;
}
