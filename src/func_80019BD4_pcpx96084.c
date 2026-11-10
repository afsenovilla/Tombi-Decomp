// VERSION PCPX_96084 tomba 80019BD4 212
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 object pool init, the shape of PAL initObjectPoolLayer7 (src/initObjectPoolLayer7.c): 80 objects of 0xF0
// bytes, category 2; free list pointer at scratchpad 0x2E8, lists at 0x308 / 0x3AC.
extern int * D_1F8002E8;
#define FREE_OBJECTS D_1F8002E8
extern short D_1F8002C6;
#define FREE_OBJECT_COUNT D_1F8002C6
extern char * D_1F800308;
#define OBJECT_LIST D_1F800308
extern char * D_1F8003AC;
#define OBJECT_LIST_SAVED D_1F8003AC
extern short D_1F80039A;
#define OBJECT_COUNT_SAVED D_1F80039A
extern short D_1F8002D6;
#define OBJECT_COUNT D_1F8002D6
extern unsigned char D_800E6D1C[];
extern unsigned char D_800EB72C[];
extern unsigned char D_800EB984[];
extern unsigned char D_800F55AC[];
extern void memset(void *, int, int);

void func_80019BD4_pcpx96084(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = D_800E6D1C;
    for (; i < 80; i++) {
        memset(p, 0, 0xf0);
        p += 0xf0;
    }
    q = D_800EB72C;
    FREE_OBJECTS = (int *)D_800EB984;
    for (i = 0; i < 80; i++) {
        q[0x1c] = 2;
        *--FREE_OBJECTS = (int)q;
        q -= 0xf0;
    }
    FREE_OBJECT_COUNT = 80;
    OBJECT_LIST = (char *)D_800F55AC;
    OBJECT_LIST_SAVED = (char *)D_800F55AC;
    OBJECT_COUNT_SAVED = 0;
    OBJECT_COUNT = 0;
}
#endif
