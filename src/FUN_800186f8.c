// FUNC 800186f8 76 MAIN0
// MATCHING 800186f8 76
extern short DAT_1f80023e;
extern int *DAT_1f800210;

int FUN_800186f8(void)
{
    int r;
    if (DAT_1f80023e < 1) {
        r = 0;
    } else {
        int *p = DAT_1f800210;
        DAT_1f80023e = DAT_1f80023e - 1;
        DAT_1f800210 = p + 1;
        r = *p;
    }
    return r;
}
