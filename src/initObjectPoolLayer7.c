// FUNC 80018180 212 MAIN0
// MATCHING 80018180 212
// VERSION PCPX_96084 tomba 8001A1F0 212
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 80018180 (initObjectPoolLayer7.c): PAL C of src/initObjectPoolLayer7.c (0.943 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#define FREE_OBJECTS_LAYER7 D_1F800300
extern unsigned char OBJECT_POOL_LAYER7[];
extern unsigned char DAT_800a4d74[];
extern int DAT_800a4468[];
extern char CAT7_OBJECT_LIST_BUF[];
extern int *FREE_OBJECTS_LAYER7;
extern short FREE_OBJECT_COUNT_LAYER7, CAT7_OBJECT_COUNT_SAVED, CAT7_OBJECT_COUNT;
extern char *CAT7_OBJECT_LIST, *CAT7_OBJECT_LIST_SAVED;
extern void memset_(void *, int, int);
void initObjectPoolLayer7(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = OBJECT_POOL_LAYER7;
    for (; i < 10; i++) {
        memset_(p, 0, 0x6c);
        p += 0x6c;
    }
    q = DAT_800a4d74;
    FREE_OBJECTS_LAYER7 = DAT_800a4468;
    for (i = 0; i < 10; i++) {
        q[0x1c] = 8;
        *--FREE_OBJECTS_LAYER7 = (int)q;
        q -= 0x6c;
    }
    FREE_OBJECT_COUNT_LAYER7 = 10;
    CAT7_OBJECT_LIST = CAT7_OBJECT_LIST_BUF;
    CAT7_OBJECT_LIST_SAVED = CAT7_OBJECT_LIST_BUF;
    CAT7_OBJECT_COUNT_SAVED = 0;
    CAT7_OBJECT_COUNT = 0;
}
#else
extern unsigned char OBJECT_POOL_LAYER7[];
extern unsigned char DAT_800a4d74[];
extern int DAT_800a4468[];
extern char CAT7_OBJECT_LIST_BUF[];
extern int *FREE_OBJECTS_LAYER7;
extern short FREE_OBJECT_COUNT_LAYER7, CAT7_OBJECT_COUNT_SAVED, CAT7_OBJECT_COUNT;
extern char *CAT7_OBJECT_LIST, *CAT7_OBJECT_LIST_SAVED;
extern void memset_(void *, int, int);
void initObjectPoolLayer7(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = OBJECT_POOL_LAYER7;
    for (; i < 10; i++) {
        memset_(p, 0, 0x6c);
        p += 0x6c;
    }
    q = DAT_800a4d74;
    FREE_OBJECTS_LAYER7 = DAT_800a4468;
    for (i = 0; i < 10; i++) {
        q[0x1c] = 7;
        *--FREE_OBJECTS_LAYER7 = (int)q;
        q -= 0x6c;
    }
    FREE_OBJECT_COUNT_LAYER7 = 10;
    CAT7_OBJECT_LIST = CAT7_OBJECT_LIST_BUF;
    CAT7_OBJECT_LIST_SAVED = CAT7_OBJECT_LIST_BUF;
    CAT7_OBJECT_COUNT_SAVED = 0;
    CAT7_OBJECT_COUNT = 0;
}
#endif
