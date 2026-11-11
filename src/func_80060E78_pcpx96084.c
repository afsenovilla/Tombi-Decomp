// VERSION PCPX_96084 tomba 80060E78 240
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// drawCategory4Objects shape over another scratchpad list (0x1F8002D6 / 0x1F800308, saved at 0x1F80039A / 0x1F8003AC) that skips objects whose byte +1 is clear.
#define DRAW_FROZEN D_1F800126
#define CAT4_OBJECT_COUNT_SAVED D_1F80039A
#define CAT4_OBJECT_LIST_SAVED D_1F8003AC
#define CAT4_OBJECT_COUNT D_1F8002D6
#define CAT4_OBJECT_LIST D_1F800308
#define DAT_1f80024c (*(short *)&CAT4_OBJECT_COUNT)
extern short DRAW_FROZEN;
extern short CAT4_OBJECT_COUNT_SAVED;
extern unsigned short CAT4_OBJECT_COUNT;
extern int ** CAT4_OBJECT_LIST;
extern int **CAT4_OBJECT_LIST_SAVED;
extern void func_8005A3E0(int *o);

void func_80060E78_pcpx96084(void)
{
    if (DRAW_FROZEN != 0) {
        int n = CAT4_OBJECT_COUNT_SAVED;
        int **p = CAT4_OBJECT_LIST_SAVED;
        for (; n != 0; n--) {
            int *o = *p;
            p++;
            func_8005A3E0(o);
        }
    } else {
        unsigned short n = CAT4_OBJECT_COUNT;
        int **q = CAT4_OBJECT_LIST;
        CAT4_OBJECT_COUNT_SAVED = n;
        CAT4_OBJECT_LIST_SAVED = q;
        if (n != 0) do {
            int *o = *CAT4_OBJECT_LIST++;
            CAT4_OBJECT_COUNT--;
            if (((unsigned char *)o)[1]) func_8005A3E0(o);
        } while (DAT_1f80024c != 0);
    }
}
#endif
