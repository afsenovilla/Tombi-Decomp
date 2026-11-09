// FUNC 80043d80 316 MAIN0
// MATCHING 80043d80 316
// VERSION PCPX_96084 tomba 8006A454 316
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 80043D80 (playerContactCat4Type0B.c): PAL C of src/playerContactCat4Type0B.c (0.962 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct {
    char p00[0x14]; short s14; unsigned short s16;
    char p1[0x40 - 0x18]; H *h40; H *h44;
    char p2[0x69 - 0x48]; char b69;
    char p3[0x6c - 0x6a]; unsigned short s6c; short s6e; unsigned short s70; short s72;
    char p4[0x7e - 0x74]; short s7e;
    char p5[0x98 - 0x80]; unsigned char b9c;
    char p6[0xac - 0x9d]; short sb0;
} TO;

int playerContactCat4Type0B(TO *a, TO *b)
{
    char pad;
    int t;
    if ((unsigned short)(a->h44->s2 - b->h44->s2 + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(a->h40->s2 - b->h40->s2 + (b->s6c + a->s6c)) > (b->s6e + a->s6e)) return 0;
    t = a->s16 - b->s16;
    if ((unsigned short)(t + (b->s70 + a->s70)) > (a->s72 + b->s72)) return 0;
    if ((t << 16) <= 0) {
        if (a->b9c & 1) return 0;
        a->sb0 = 0;
        a->s16 = b->s16 - (b->s70 + a->s70);
        a->s14 = 0;
        a->b69 = 1;
        a->s7e = 0;
        b->b69 = 1;
        return 1;
    }
    a->s16 = b->s16 + ((b->s72 - b->s70) + (a->s72 - a->s70));
    if (a->s7e < 0) a->s7e = 0;
    return 3;
}
#else
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct {
    char p00[0x14]; short s14; unsigned short s16;
    char p1[0x40 - 0x18]; H *h40; H *h44;
    char p2[0x69 - 0x48]; char b69;
    char p3[0x6c - 0x6a]; unsigned short s6c; short s6e; unsigned short s70; short s72;
    char p4[0x7e - 0x74]; short s7e;
    char p5[0x9c - 0x80]; unsigned char b9c;
    char p6[0xb0 - 0x9d]; short sb0;
} TO;

int playerContactCat4Type0B(TO *a, TO *b)
{
    char pad;
    int t;
    if ((unsigned short)(a->h44->s2 - b->h44->s2 + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(a->h40->s2 - b->h40->s2 + (b->s6c + a->s6c)) > (b->s6e + a->s6e)) return 0;
    t = a->s16 - b->s16;
    if ((unsigned short)(t + (b->s70 + a->s70)) > (a->s72 + b->s72)) return 0;
    if ((t << 16) <= 0) {
        if (a->b9c & 1) return 0;
        a->sb0 = 0;
        a->s16 = b->s16 - (b->s70 + a->s70);
        a->s14 = 0;
        a->b69 = 1;
        a->s7e = 0;
        b->b69 = 1;
        return 1;
    }
    a->s16 = b->s16 + ((b->s72 - b->s70) + (a->s72 - a->s70));
    if (a->s7e < 0) a->s7e = 0;
    return 3;
}
#endif
