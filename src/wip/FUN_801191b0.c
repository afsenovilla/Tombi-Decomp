// FUNC 801191b0 188 X000
extern char *FUN_80018448(void);

void FUN_801191b0(int a, int b, int c, int d)
{
    int i;
    char *n;
    int bb = b << 16;
    int cc = c << 16;
    int dd = d << 16;
    for (i = 0; i < 2; i++) {
        n = FUN_80018448();
        if (n != 0) {
            n[2] = 3;
            n[10] = 5;
            n[0] = 1;
            n[3] = i;
            *(short *)(n + 0x2c) = 0;
            *(int *)(n + 0x10) = bb;
            *(int *)(n + 0x14) = cc;
            *(int *)(n + 0x18) = dd;
            if ((a << 16) == 0)
                n[12] = 1;
            else
                n[12] = 0;
        }
    }
}
