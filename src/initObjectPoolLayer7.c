// FUNC 80018180 212 MAIN0
// MATCHING 80018180 212
// VERSION PCPX_96084 tomba 8001A1F0 212
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: 10 objects of 0x6C bytes, category 8; free list pointer at scratchpad 0x300, lists at 0x320 /
// 0x3C4 (the scratchpad words as D_1F80xxxx symbols, so that every access has its own lui).
extern int * D_1F800300;
#define FREE_OBJECTS D_1F800300
extern short D_1F8002D2;
#define FREE_OBJECT_COUNT D_1F8002D2
extern char * D_1F800320;
#define OBJECT_LIST D_1F800320
extern char * D_1F8003C4;
#define OBJECT_LIST_SAVED D_1F8003C4
extern short D_1F8003A6;
#define OBJECT_COUNT_SAVED D_1F8003A6
extern short D_1F8002E2;
#define OBJECT_COUNT D_1F8002E2
extern unsigned char D_800F4AD8[];
extern unsigned char D_800F4EA4[];
extern unsigned char D_800EB844[];
extern unsigned char D_800F546C[];
extern void memset(void *, int, int);

void initObjectPoolLayer7(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = D_800F4AD8;
    for (; i < 10; i++) {
        memset(p, 0, 0x6c);
        p += 0x6c;
    }
    q = D_800F4EA4;
    FREE_OBJECTS = (int *)D_800EB844;
    for (i = 0; i < 10; i++) {
        q[0x1c] = 8;
        *--FREE_OBJECTS = (int)q;
        q -= 0x6c;
    }
    FREE_OBJECT_COUNT = 10;
    OBJECT_LIST = (char *)D_800F546C;
    OBJECT_LIST_SAVED = (char *)D_800F546C;
    OBJECT_COUNT_SAVED = 0;
    OBJECT_COUNT = 0;
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
