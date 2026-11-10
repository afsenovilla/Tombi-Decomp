// VERSION PCPX_96084 tomba 80019B00 212
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 object pool init, the shape of PAL initObjectPoolLayer7 (src/initObjectPoolLayer7.c): 4 objects of 0xE4
// bytes, category 1; free list pointer at scratchpad 0x2E4, lists at 0x304 / 0x3A8.
extern int * D_1F8002E4;
#define FREE_OBJECTS D_1F8002E4
extern short D_1F8002C4;
#define FREE_OBJECT_COUNT D_1F8002C4
extern char * D_1F800304;
#define OBJECT_LIST D_1F800304
extern char * D_1F8003A8;
#define OBJECT_LIST_SAVED D_1F8003A8
extern short D_1F800398;
#define OBJECT_COUNT_SAVED D_1F800398
extern short D_1F8002D4;
#define OBJECT_COUNT D_1F8002D4
extern unsigned char D_800FDA88[];
extern unsigned char D_800FDD34[];
extern unsigned char D_800EBC38[];
extern unsigned char D_800F77C8[];
extern void memset(void *, int, int);

void func_80019B00_pcpx96084(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = D_800FDA88;
    for (; i < 4; i++) {
        memset(p, 0, 0xe4);
        p += 0xe4;
    }
    q = D_800FDD34;
    FREE_OBJECTS = (int *)D_800EBC38;
    for (i = 0; i < 4; i++) {
        q[0x1c] = 1;
        *--FREE_OBJECTS = (int)q;
        q -= 0xe4;
    }
    FREE_OBJECT_COUNT = 4;
    OBJECT_LIST = (char *)D_800F77C8;
    OBJECT_LIST_SAVED = (char *)D_800F77C8;
    OBJECT_COUNT_SAVED = 0;
    OBJECT_COUNT = 0;
}
#endif
