// FUNC 80118d8c 228 X000
// MATCHING 80118d8c 228
extern char *FUN_80018448(void);

void FUN_80118d8c(char *o, short b, short c, int d)
{
    int i = 0;
    int cc = c;
    int dd = d << 16;
    int x = b;
    char *n;
    unsigned short t;
    do {
        n = FUN_80018448();
        if (n != 0) {
            n[0] = 1;
            n[2] = 2;
            *(int *)(n + 0x10) = x << 16;
            *(int *)(n + 0x14) = (cc + (i % 2) * -8) * 0x10000;
            *(int *)(n + 0x18) = dd;
            t = *(unsigned short *)(o + 0x1e);
            n[0xd] = 0;
            n[10] = 0;
            n[3] = i;
            *(signed char *)&n[0xf] = -2;
            *(unsigned short *)(n + 0x1e) = t;
            *(int *)(n + 0x3c) = *(int *)(o + 0x3c);
        }
        i++;
        x -= 2;
    } while (i < 6);
}
