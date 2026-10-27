// FUNC 80057288 224 MAIN0
// MATCHING 80057288 224
extern short DAT_1f8001c6;
extern short CAT4_OBJECT_COUNT_SAVED;
extern short DAT_1f80024c;
extern unsigned short CAT4_OBJECT_COUNT;
extern int ** CAT4_OBJECT_LIST;
extern int **CAT4_OBJECT_LIST_SAVED;
extern void drawObject(int *o);

void drawCategory4Objects(void)
{
    if (DAT_1f8001c6 != 0) {
        int n = CAT4_OBJECT_COUNT_SAVED;
        int **p = CAT4_OBJECT_LIST_SAVED;
        for (; n != 0; n--) {
            int *o = *p;
            p++;
            drawObject(o);
        }
    } else {
        unsigned short n = CAT4_OBJECT_COUNT;
        int **q = CAT4_OBJECT_LIST;
        CAT4_OBJECT_COUNT_SAVED = n;
        CAT4_OBJECT_LIST_SAVED = q;
        if (n != 0) do {
            int *o = *CAT4_OBJECT_LIST++;
            CAT4_OBJECT_COUNT--;
            drawObject(o);
        } while (DAT_1f80024c != 0);
    }
}
