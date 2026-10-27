// FUNC 80057288 224 MAIN0
// MATCHING 80057288 224
extern short DAT_1f8001c6;
extern short DAT_1f800254;
extern short DAT_1f80024c;
extern unsigned short CAT4_OBJECT_COUNT;
extern int ** DAT_1f800224;
extern int **DAT_1f800268;
extern void drawObject(int *o);

void drawCategory4Objects(void)
{
    if (DAT_1f8001c6 != 0) {
        int n = DAT_1f800254;
        int **p = DAT_1f800268;
        for (; n != 0; n--) {
            int *o = *p;
            p++;
            drawObject(o);
        }
    } else {
        unsigned short n = CAT4_OBJECT_COUNT;
        int **q = DAT_1f800224;
        DAT_1f800254 = n;
        DAT_1f800268 = q;
        if (n != 0) do {
            int *o = *DAT_1f800224++;
            CAT4_OBJECT_COUNT--;
            drawObject(o);
        } while (DAT_1f80024c != 0);
    }
}
