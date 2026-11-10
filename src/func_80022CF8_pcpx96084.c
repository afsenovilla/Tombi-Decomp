// VERSION PCPX_96084 tomba 80022CF8 104
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 movieInitDecodeEnv (src/movieInitDecodeEnv.c): the last field is a global here, not 0xE0.
extern char D_80100DC4[], D_80110DC4[], D_80120DC4[], D_80122DC4[];
extern int D_800C8234;

typedef struct {
    void *p0, *p4;
    int i8;
    void *pc, *p10;
    int i14;
    short s18, s1a;
    char pad[4];
    short s20, s22;
    char pad2[0x30 - 0x24];
    short s30, s32;
    int i34;
} S80022CF8;

void func_80022CF8_pcpx96084(o, a, b, c, d)
    S80022CF8 *o;
    unsigned short a;
    short b, c;
    int d;
{
    o->p0 = D_80100DC4;
    o->p4 = D_80110DC4;
    o->pc = D_80120DC4;
    o->i8 = 0;
    o->p10 = D_80122DC4;
    o->i14 = 0;
    o->s18 = a;
    o->s1a = b;
    o->s20 = c;
    o->i34 = 0;
    o->s30 = 0x10;
    o->s22 = d;
    o->s32 = D_800C8234;
}
#endif
