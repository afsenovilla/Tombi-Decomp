// FUNC 80121a5c 132 X000
extern char *FUN_80018448(void);
char *FUN_80121a5c(int a, int x, int y, int z)
{
    char *p = FUN_80018448();
    if (p) {
        p[0] = 1; p[2] = 0x44; *(int *)(p + 0x10) = x << 16; *(int *)(p + 0x14) = y << 16; p[0xc] = a; *(int *)(p + 0x18) = z << 16;
    }
    return p;
}
