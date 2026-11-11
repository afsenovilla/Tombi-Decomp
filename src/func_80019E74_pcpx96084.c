// VERSION PCPX_96084 tomba 80019E74 212
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 object pool init, the shape of PAL initObjectPoolLayer7 (src/initObjectPoolLayer7.c): 150 objects of 0xF0
// bytes, category 5; free list pointer at scratchpad 0x2F4, lists at 0x314 / 0x3B8.
extern int * D_1F8002F4;
#define FREE_OBJECTS D_1F8002F4
extern short D_1F8002CC;
#define FREE_OBJECT_COUNT D_1F8002CC
extern char * D_1F800314;
#define OBJECT_LIST D_1F800314
extern char * D_1F8003B8;
#define OBJECT_LIST_SAVED D_1F8003B8
extern short D_1F8003A0;
#define OBJECT_COUNT_SAVED D_1F8003A0
extern short D_1F8002DC;
#define OBJECT_COUNT D_1F8002DC
extern unsigned char D_800EBDB8[];
extern unsigned char D_800F4968[];
extern unsigned char D_800EBBE4[];
extern unsigned char D_800F7750[];
extern void memset(void *, int, int);

void func_80019E74_pcpx96084(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = D_800EBDB8;
    for (; i < 150; i++) {
        memset(p, 0, 0xf0);
        p += 0xf0;
    }
    q = D_800F4968;
    FREE_OBJECTS = (int *)D_800EBBE4;
    for (i = 0; i < 150; i++) {
        q[0x1c] = 5;
        *--FREE_OBJECTS = (int)q;
        q -= 0xf0;
    }
    FREE_OBJECT_COUNT = 150;
    OBJECT_LIST = (char *)D_800F7750;
    OBJECT_LIST_SAVED = (char *)D_800F7750;
    OBJECT_COUNT_SAVED = 0;
    OBJECT_COUNT = 0;
}
#endif
