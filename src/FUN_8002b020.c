// FUNC 8002b020 148 MAIN0
// MATCHING 8002b020 148
extern char DAT_800b1828[];
extern void (*PTR_DAT_80079b88[])(char *);
extern int DAT_1f800198;

void FUN_8002b020(void)
{
    char *p = DAT_800b1828;
    DAT_1f800198 = 0;
    do {
        if (*p != 0)
            PTR_DAT_80079b88[(unsigned char)p[2]](p);
        DAT_1f800198++;
        p += 0xd4;
    } while (DAT_1f800198 < 0x2d);
}
