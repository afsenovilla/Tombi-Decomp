// FUNC 80057288 224 MAIN0
// MATCHING 80057288 224
#if defined(VERSION_PCPX_96084) /* PrePre 8 scratchpad layout */
#define DRAW_FROZEN D_1F800126
#define CAT4_OBJECT_LIST D_1F80031C
#define CAT4_OBJECT_COUNT D_1F8002E0
#define DAT_1f80024c (*(short *)&CAT4_OBJECT_COUNT)
#endif
extern short DRAW_FROZEN;
extern short CAT4_OBJECT_COUNT_SAVED;
#if !defined(VERSION_PCPX_96084)
extern short DAT_1f80024c;
#endif
extern unsigned short CAT4_OBJECT_COUNT;
extern int ** CAT4_OBJECT_LIST;
extern int **CAT4_OBJECT_LIST_SAVED;
extern void drawObject(int *o);

void drawCategory4Objects(void)
{
    if (DRAW_FROZEN != 0) {
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
