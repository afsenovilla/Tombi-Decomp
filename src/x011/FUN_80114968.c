// FUNC 80114968 160 X011
// MATCHING 80114968 160
extern char *FUN_800183b8(void);
extern unsigned char DAT_8009c959;

void FUN_80114968(int unused, char sub, int x, int y, int z)
{
    char *p = FUN_800183b8();
    if (p != 0) {
        p[0] = 1;
        p[2] = 0x1f;
        p[3] = sub;
        **(int **)(p + 0x40) = x << 16;
        *(int *)(p + 0x14) = y << 16;
        **(int **)(p + 0x44) = z << 16;
        { unsigned char *c = &DAT_8009c959; *c = *c + 1; }
    }
}
