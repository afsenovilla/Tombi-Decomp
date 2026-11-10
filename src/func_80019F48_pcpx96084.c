// VERSION PCPX_96084 tomba 80019F48 212
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 object pool init, the shape of PAL initObjectPoolLayer7 (src/initObjectPoolLayer7.c): 50 objects of 0xF0
// bytes, category 9; free list pointer at scratchpad 0x374, lists at 0x3CC / 0x3C8.
extern int * D_1F800374;
#define FREE_OBJECTS D_1F800374
extern short D_1F800370;
#define FREE_OBJECT_COUNT D_1F800370
extern char * D_1F8003CC;
#define OBJECT_LIST D_1F8003CC
extern char * D_1F8003C8;
#define OBJECT_LIST_SAVED D_1F8003C8
extern short D_1F8003D2;
#define OBJECT_COUNT_SAVED D_1F8003D2
extern short D_1F8003D0;
#define OBJECT_COUNT D_1F8003D0
extern unsigned char D_800FDEE0[];
extern unsigned char D_80100CD0[];
extern unsigned char D_800F51EC[];
extern void memset(void *, int, int);

void func_80019F48_pcpx96084(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = D_800FDEE0;
    for (; i < 50; i++) {
        memset(p, 0, 0xf0);
        p += 0xf0;
    }
    q = D_80100CD0;
    FREE_OBJECTS = (int *)D_800F51EC;
    for (i = 0; i < 50; i++) {
        q[0x1c] = 9;
        *--FREE_OBJECTS = (int)q;
        q -= 0xf0;
    }
    FREE_OBJECT_COUNT = 50;
    OBJECT_LIST = (char *)D_800FDEE0;
    OBJECT_LIST_SAVED = (char *)D_800FDEE0;
    OBJECT_COUNT_SAVED = 0;
    OBJECT_COUNT = 0;
}
#endif
