// FUNC 8004c0dc 148 MAIN0
// MATCHING 8004c0dc 148
extern int DAT_1f800198;
extern unsigned char DAT_800a6268[];
extern void (*PTR_8007bf34[])(unsigned char *);

void FUN_8004c0dc(void)
{
    unsigned char *p = DAT_800a6268;
    DAT_1f800198 = 0;
    do {
        if (*p != 0) {
            PTR_8007bf34[p[2]](p);
        }
        DAT_1f800198 = DAT_1f800198 + 1;
        p += 0x3c;
    } while (DAT_1f800198 < 10);
}
