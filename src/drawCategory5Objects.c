// FUNC 800571a8 224 MAIN0
// MATCHING 800571a8 224
// FLAGS -O2 -G0 -fno-schedule-insns
extern short DAT_1f8001c6, DAT_1f800256;
extern unsigned *DAT_1f80026c;
extern short CAT5_OBJECT_COUNT; extern unsigned *CAT5_OBJECT_LIST;
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
        DAT_1f800256 = CAT5_OBJECT_COUNT;
        DAT_1f80026c = CAT5_OBJECT_LIST;
        if (CAT5_OBJECT_COUNT != 0) {
            do {
                q = CAT5_OBJECT_LIST; CAT5_OBJECT_LIST = q + 1; s = CAT5_OBJECT_COUNT; x = *q; CAT5_OBJECT_COUNT = s - 1; drawObject(x);
            } while (CAT5_OBJECT_COUNT != 0);
        }
    }
}
