// FUNC 80026a10 492 MAIN0
typedef struct { unsigned int score; char p0[3]; unsigned char b7; char p1[0xc]; unsigned char b14; } DS;
extern DS D_8009c96c;
extern unsigned int T_8007978c[];
extern unsigned char B_800b144c[];

void FUN_80026a10(int add)
{
    DS *d;
    int i;
    unsigned int old, new, n;
    unsigned int *p;
    unsigned char *q;
    d = &D_8009c96c;
    i = 0;
    old = d->score;
    p = T_8007978c;
    new = old + add;
    do {
        if (old < *p && new >= *p) {
            d->b7 = i + 2;
            d->b14 += 3;
            if (d->b14 >= 100) d->b14 = 99;
        }
        p++;
        i++;
    } while (*p != -1);
    n = D_8009c96c.score + add;
    D_8009c96c.score = n;
    q = B_800b144c;
    *q++ = n / 10000000 % 10;
    *q++ = n / 1000000 % 10;
    *q++ = n / 100000 % 10;
    *q++ = n / 10000 % 10;
    *q++ = n / 1000 % 10;
    *q++ = n / 100 % 10;
    *q++ = n / 10 % 10;
    *q = n % 10;
}
