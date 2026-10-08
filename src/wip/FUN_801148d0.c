// FUNC 801148d0 152 X000
extern char *FUN_800183b8(void);

void FUN_801148d0(int a, char b, int c, int d, int e)
{
    char *n = FUN_800183b8();
    if (n != 0) {
        n[0] = 1;
        n[2] = 0x1f;
        n[3] = b;
        *(int *)(n + 0x14) = d << 16;
        **(int **)(n + 0x40) = c << 16;
        **(int **)(n + 0x44) = e << 16;
        *(int *)(n + 0x90) = a;
    }
}
