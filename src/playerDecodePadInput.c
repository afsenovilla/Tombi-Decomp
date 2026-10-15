// FUNC 80025d90 432 MAIN0
// MATCHING 80025d90 432
/* Pad input decode: two 4-way axis fields (0x10/0x20/0x40/0x80 bits) from the byte globals DAT_8009d618 and
   DAT_8009d610.b9, merged into DAT_8009d670; press/release masks to the scratchpad.
   Notes (n65): `a` must be an int (the game's `andi a0,a3,0xff` is a zero_extendqisi2 that survives because the
   lbu register is used again in the `c < 0x80` test); it is assigned after the pad mask so its andi takes the
   luid right before the pad lhu; `d` is a 3-set int per axis (global pseudo: v1 in the first axis because the
   pad value is live in v0, v0 in the second); the dead `a = 0` after the last use stops cse from reusing `a` for
   the `c < 0x80` test (cse follows the two beqz around the j blocks) without emitting code; without it the test
   compiles to `sltiu v0,a0,0x80` (`-fno-cse-follow-jumps` would do the same). The DAT_8009d674 store written
   first puts the lhu of DAT_800a6114 before the lbu (no second extern name needed). */
typedef struct { char p0[9]; unsigned char b9; } G;
extern unsigned char DAT_8009d618;
extern G DAT_8009d610;
extern unsigned short DAT_8009d670;
extern volatile unsigned short DAT_8009d674;
extern unsigned short DAT_800a6114;
extern unsigned short DAT_1f8001fc;
extern unsigned short DAT_1f8001fe;

#define AXIS(v, c, a, hi, lo) ({                                  \
    int d;                                                      \
    int r;                                                      \
    d = a - 0x50;                                               \
    if ((unsigned)d < 0x60) r = 0;                              \
    else {                                                      \
        d = a - 0x30;                                           \
        if ((unsigned short)d < 0xa0) { (v) = 1; { int t = ((unsigned short)d < 0x50) ? hi : lo; r = t; } } \
        else {                                                  \
            d = a - 0x10;                                       \
            (a) = 0;                                            \
            if ((unsigned short)d < 0xe0) { (v) = 2; { int t = ((unsigned short)d < 0x70) ? hi : lo; r = t; } } \
            else { (v) = 3; { int t = (c < 0x80) ? hi : lo; r = t; } }         \
        }                                                       \
    }                                                           \
    r; })

void playerDecodePadInput(void)
{
    unsigned short m;
    unsigned char c1, c2;
    unsigned int a1, a2;
    volatile unsigned short *pad;
    G *g = &DAT_8009d610;
    DAT_8009d674 = DAT_800a6114;
    c1 = DAT_8009d618;
    DAT_8009d618 = 0;
    m = *(volatile unsigned short *)&DAT_8009d670 & 0xff0f;
    a1 = c1;
    m |= AXIS(DAT_8009d618, c1, a1, 0x80, 0x20);
    c2 = g->b9;
    g->b9 = 0;
    a2 = c2;
    m |= AXIS(g->b9, c2, a2, 0x10, 0x40);
    pad = &DAT_8009d670;
    *pad = m;
    DAT_1f8001fc = *pad & ~DAT_8009d674;
    DAT_1f8001fe = DAT_8009d674 & ~*pad;
    DAT_800a6114 = *pad;
}
