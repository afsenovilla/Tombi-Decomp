// FUNC 800571a8 224 MAIN0
extern short DAT_1f8001c6, DAT_1f800256, DAT_1f800248;
extern unsigned *DAT_1f80026c, *DAT_1f800228;
extern void FUN_800500c4(unsigned);

void FUN_800571a8(void)
{
    int n;
    unsigned *p, *q;
    if (DAT_1f8001c6 != 0) {
        p = DAT_1f80026c;
        n = DAT_1f800256;
        if (n != 0) {
            do {
                FUN_800500c4(*p++);
            } while (--n != 0);
        }
    } else {
        DAT_1f800256 = DAT_1f800248;
        DAT_1f80026c = DAT_1f800228;
        if (DAT_1f800248 != 0) {
            do {
                q = DAT_1f800228;
                DAT_1f800228 = q + 1;
                DAT_1f800248 = DAT_1f800248 - 1;
                FUN_800500c4(*q);
            } while (DAT_1f800248 != 0);
        }
    }
}
