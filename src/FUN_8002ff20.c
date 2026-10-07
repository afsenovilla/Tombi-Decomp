// FUNC 8002ff20 144 MAIN0
// MATCHING 8002ff20 144
extern char *FUN_80018448();
extern int DAT_800a6048, DAT_800a604c, DAT_800a6050;

void FUN_8002ff20(char a, char b, char c)
{
    int t;
    char *p = FUN_80018448();
    if (p != 0) {
        p[0] = 1;
        p[2] = 0x4e;
        p[3] = a;
        p[0xc] = b;
        *(int *)(p + 0x10) = DAT_800a6048;
        *(int *)(p + 0x14) = DAT_800a604c;
        t = DAT_800a6050;
        p[5] = c;
        *(int *)(p + 0x18) = t;
    }
}
