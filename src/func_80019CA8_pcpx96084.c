// VERSION PCPX_96084 tomba 80019CA8 212
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 object pool init, the shape of PAL initObjectPoolLayer7 (src/initObjectPoolLayer7.c): 2 objects of 0xDC
// bytes, category 3; free list pointer at scratchpad 0x2EC, lists at 0x30C / 0x3B0.
extern int * D_1F8002EC;
#define FREE_OBJECTS D_1F8002EC
extern short D_1F8002C8;
#define FREE_OBJECT_COUNT D_1F8002C8
extern char * D_1F80030C;
#define OBJECT_LIST D_1F80030C
extern char * D_1F8003B0;
#define OBJECT_LIST_SAVED D_1F8003B0
extern short D_1F80039C;
#define OBJECT_COUNT_SAVED D_1F80039C
extern short D_1F8002D8;
#define OBJECT_COUNT D_1F8002D8
extern unsigned char D_800FCB10[];
extern unsigned char D_800FCBEC[];
extern unsigned char D_800EB98C[];
extern unsigned char D_800F55B8[];
extern void memset(void *, int, int);

void func_80019CA8_pcpx96084(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = D_800FCB10;
    for (; i < 2; i++) {
        memset(p, 0, 0xdc);
        p += 0xdc;
    }
    q = D_800FCBEC;
    FREE_OBJECTS = (int *)D_800EB98C;
    for (i = 0; i < 2; i++) {
        q[0x1c] = 3;
        *--FREE_OBJECTS = (int)q;
        q -= 0xdc;
    }
    FREE_OBJECT_COUNT = 2;
    OBJECT_LIST = (char *)D_800F55B8;
    OBJECT_LIST_SAVED = (char *)D_800F55B8;
    OBJECT_COUNT_SAVED = 0;
    OBJECT_COUNT = 0;
}
#endif
