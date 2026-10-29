// FUNC 80057dfc 224 MAIN0
// MATCHING 80057dfc 224
#if defined(VERSION_PCPX_96084) /* PrePre 8 draws category 8 from the PAL category 7 list (0x1F80022C/240) */
#define CAT8_OBJECT_LIST CAT7_OBJECT_LIST
#define DAT_1f800230 SCR_22C
#define CAT8_OBJECT_COUNT CAT7_OBJECT_COUNT
#define DAT_1f800242 SCR_240
#define CAT8_OBJECT_COUNT_s DAT_1f800240
#endif
extern short DAT_1f8001c6;
extern short CAT8_OBJECT_COUNT_SAVED;
extern int *CAT8_OBJECT_LIST_SAVED;
extern int *DAT_1f800230;
extern int * volatile CAT8_OBJECT_LIST;
extern volatile unsigned short CAT8_OBJECT_COUNT;
extern unsigned short DAT_1f800242;
extern short CAT8_OBJECT_COUNT_s[];
extern void drawObject(int);

void drawCategory8Objects(void)
{
    int n;
    int *p, *q;
    int x;
    if (DAT_1f8001c6 != 0) {
        n = CAT8_OBJECT_COUNT_SAVED;
        p = CAT8_OBJECT_LIST_SAVED;
        while (n != 0) {
            drawObject(*p++);
            n--;
        }
    } else {
        if (1) {
        CAT8_OBJECT_COUNT_SAVED = DAT_1f800242;
        CAT8_OBJECT_LIST_SAVED = DAT_1f800230;
        if (CAT8_OBJECT_COUNT_SAVED != 0) {
            do {
                q = CAT8_OBJECT_LIST;
                CAT8_OBJECT_LIST = q + 1;
                x = *q;
                CAT8_OBJECT_COUNT = CAT8_OBJECT_COUNT - 1;
                drawObject(x);
            } while (CAT8_OBJECT_COUNT_s[0] != 0);
        }
        }
    }
}
