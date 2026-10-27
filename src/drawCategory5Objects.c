// FUNC 800571a8 224 MAIN0
// MATCHING 800571a8 224
// FLAGS -O2 -G0 -fno-schedule-insns
extern short DAT_1f8001c6, CAT5_OBJECT_COUNT_SAVED;
extern unsigned *CAT5_OBJECT_LIST_SAVED;
extern short CAT5_OBJECT_COUNT; extern unsigned *CAT5_OBJECT_LIST;
extern void drawObject(unsigned);

void drawCategory5Objects(void)
{
    int n;
    unsigned *p, *q;
    unsigned x;
    short s;
    if (DAT_1f8001c6 != 0) {
        n = CAT5_OBJECT_COUNT_SAVED;
        p = CAT5_OBJECT_LIST_SAVED;
        if (n != 0) {
            for (;;) {
                x = *p++; n--; drawObject(x); if (n == 0) break;
            }
        }
    } else {
        CAT5_OBJECT_COUNT_SAVED = CAT5_OBJECT_COUNT;
        CAT5_OBJECT_LIST_SAVED = CAT5_OBJECT_LIST;
        if (CAT5_OBJECT_COUNT != 0) {
            do {
                q = CAT5_OBJECT_LIST; CAT5_OBJECT_LIST = q + 1; s = CAT5_OBJECT_COUNT; x = *q; CAT5_OBJECT_COUNT = s - 1; drawObject(x);
            } while (CAT5_OBJECT_COUNT != 0);
        }
    }
}
