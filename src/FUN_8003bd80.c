// FUNC 8003bd80 148 MAIN0
// MATCHING 8003bd80 148
extern int g198;
extern char DAT_800b1478[];
extern void (*PTR_8007a10c[])(char *);
void FUN_8003bd80(void)
{
    char *p = DAT_800b1478;
    g198 = 0;
    do {
        if (*p != 0)
            PTR_8007a10c[(unsigned char)p[2]](p);
        g198 = g198 + 1;
        p += 0xec;
    } while (g198 < 4);
}
