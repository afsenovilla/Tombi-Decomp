// FUNC 800571a8 224 MAIN0
// MATCHING 800571a8 224
// FLAGS -O2 -G0 -fno-schedule-insns
extern short DAT_1f8001c6, DAT_1f800256;
extern unsigned *DAT_1f80026c;
extern short DAT_1f800248; extern unsigned *DAT_1f800228;
extern void drawObject(unsigned);

void drawCategory5Objects(void)
{
    int n;
    unsigned *p, *q;
    unsigned x;
    short s;
    if (DAT_1f8001c6 != 0) {
        n = DAT_1f800256;
        p = DAT_1f80026c;
        if (n != 0) {
            for (;;) {
                x = *p++; n--; drawObject(x); if (n == 0) break;
            }
        }
    } else {
        DAT_1f800256 = DAT_1f800248;
        DAT_1f80026c = DAT_1f800228;
        if (DAT_1f800248 != 0) {
            do {
                q = DAT_1f800228; DAT_1f800228 = q + 1; s = DAT_1f800248; x = *q; DAT_1f800248 = s - 1; drawObject(x);
            } while (DAT_1f800248 != 0);
        }
    }
}
