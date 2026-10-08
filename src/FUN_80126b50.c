// FUNC 80126b50 272 X000
// MATCHING 80126b50 272
extern short FUN_800482ec(void);
extern short FUN_8004874c(unsigned char *, unsigned char *);
extern short DAT_1f80019e;

void FUN_80126b50(unsigned char *a, unsigned char *b)
{
    short r;
    unsigned char *c;
    unsigned x;
    if (*(int *)(b + 0x94) == 0) {
        r = FUN_800482ec();
        if (r == 0)
            return;
        if (r == 2) {
            if (a[0x68] == 0)
                return;
            x = *(unsigned short *)(a + 0x2e);
            c = *(unsigned char **)(b + 0x28);
            goto tail;
        }
        if (r != 1)
            return;
        if (a[0] == 5)
            a[0x69] = 0;
        return;
    }
    r = FUN_8004874c(a, b);
    if (r != 2)
        return;
    if (a[0x68] == 0)
        return;
    c = b;
    if (*(unsigned short *)(c + 0x2c) == 0)
        return;
    x = *(unsigned short *)(a + 0x2e);
    c = *(unsigned char **)(c + 0x28);
tail:
    *(unsigned short *)(c + 0x2e) = x & 1;
    c[0x6b] = *(unsigned short *)(b + 0x2c);
    c[0x68] = a[0x68];
    a[0x68] = 0;
    DAT_1f80019e = 0;
}
