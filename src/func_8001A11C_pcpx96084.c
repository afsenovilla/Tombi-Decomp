// VERSION PCPX_96084 tomba 8001A11C 212
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 object pool init, the shape of PAL initObjectPoolLayer7 (src/initObjectPoolLayer7.c): 50 objects of 0xA0
// bytes, category 7; free list pointer at scratchpad 0x2FC, lists at 0x31C / 0x3C0.
extern int * D_1F8002FC;
#define FREE_OBJECTS D_1F8002FC
extern short D_1F8002D0;
#define FREE_OBJECT_COUNT D_1F8002D0
extern char * D_1F80031C;
#define OBJECT_LIST D_1F80031C
extern char * D_1F8003C0;
#define OBJECT_LIST_SAVED D_1F8003C0
extern short D_1F8003A4;
#define OBJECT_COUNT_SAVED D_1F8003A4
extern short D_1F8002E0;
#define OBJECT_COUNT D_1F8002E0
extern unsigned char D_800F55B8[];
extern unsigned char D_800F7458[];
extern unsigned char D_800F4FD8[];
extern unsigned char D_800FCB10[];
extern void memset(void *, int, int);

void func_8001A11C_pcpx96084(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = D_800F55B8;
    for (; i < 50; i++) {
        memset(p, 0, 0xa0);
        p += 0xa0;
    }
    q = D_800F7458;
    FREE_OBJECTS = (int *)D_800F4FD8;
    for (i = 0; i < 50; i++) {
        q[0x1c] = 7;
        *--FREE_OBJECTS = (int)q;
        q -= 0xa0;
    }
    FREE_OBJECT_COUNT = 50;
    OBJECT_LIST = (char *)D_800FCB10;
    OBJECT_LIST_SAVED = (char *)D_800FCB10;
    OBJECT_COUNT_SAVED = 0;
    OBJECT_COUNT = 0;
}
#endif
