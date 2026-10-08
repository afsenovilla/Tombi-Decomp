// FUNC 801191b0 188 X000
// MATCHING 801191b0 188
extern char *FUN_80018448(void);

void FUN_801191b0(short a, short b, short c, short d)
{
    int i;
    char *n;
    for (i = 0; i < 2; i++) {
        n = FUN_80018448();
        if (n != 0) {
            n[2] = 3;
            n[10] = 5;
            n[0] = 1;
            n[3] = i;
            *(short *)(n + 0x2c) = 0;
            *(int *)(n + 0x10) = b << 16;
            *(int *)(n + 0x14) = c << 16;
            *(int *)(n + 0x18) = d << 16;
            if (a == 0)
                n[12] = 1;
            else
                n[12] = 0;
        }
    }
}
