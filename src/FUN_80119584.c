// FUNC 80119584 112 X000
extern char *FUN_80018448();

void FUN_80119584(int a, int x, int y, int z)
{
    char *p = FUN_80018448();
    if (p != 0) {
        p[0] = 1;
        p[2] = 8;
        *(int *)(p + 0x10) = x << 16;
        *(int *)(p + 0x14) = y << 16;
        *(int *)(p + 0x18) = z << 16;
    }
}
