// VERSION PCPX_96084 tomba 8001A01C 212
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 object pool init, the shape of PAL initObjectPoolLayer7 (src/initObjectPoolLayer7.c): 16 objects of 0xDC
// bytes, category 6; free list pointer at scratchpad 0x2F8, lists at 0x318 / 0x3BC.
extern int * D_1F8002F8;
#define FREE_OBJECTS D_1F8002F8
extern short D_1F8002CE;
#define FREE_OBJECT_COUNT D_1F8002CE
extern char * D_1F800318;
#define OBJECT_LIST D_1F800318
extern char * D_1F8003BC;
#define OBJECT_LIST_SAVED D_1F8003BC
extern short D_1F8003A2;
#define OBJECT_COUNT_SAVED D_1F8003A2
extern short D_1F8002DE;
#define OBJECT_COUNT D_1F8002DE
extern unsigned char D_800FCCC8[];
extern unsigned char D_800FD9AC[];
extern unsigned char D_800EBC24[];
extern unsigned char D_800F7790[];
extern void memset(void *, int, int);

void func_8001A01C_pcpx96084(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = D_800FCCC8;
    for (; i < 16; i++) {
        memset(p, 0, 0xdc);
        p += 0xdc;
    }
    q = D_800FD9AC;
    FREE_OBJECTS = (int *)D_800EBC24;
    for (i = 0; i < 16; i++) {
        q[0x1c] = 6;
        *--FREE_OBJECTS = (int)q;
        q -= 0xdc;
    }
    FREE_OBJECT_COUNT = 16;
    OBJECT_LIST = (char *)D_800F7790;
    OBJECT_LIST_SAVED = (char *)D_800F7790;
    OBJECT_COUNT_SAVED = 0;
    OBJECT_COUNT = 0;
}
#endif
