// FUNC 800186f8 76 MAIN0
// MATCHING 800186f8 76
extern short FREE_OBJECT_COUNT_UNLAYERED;
extern int *DAT_1f800210;

int allocObjectUnlayered(void)
{
    int r;
    if (FREE_OBJECT_COUNT_UNLAYERED < 1) {
        r = 0;
    } else {
        int *p = DAT_1f800210;
        FREE_OBJECT_COUNT_UNLAYERED = FREE_OBJECT_COUNT_UNLAYERED - 1;
        DAT_1f800210 = p + 1;
        r = *p;
    }
    return r;
}
