// FUNC 80025d90 432 MAIN0
typedef struct { char p0[9]; unsigned char b9; } G;
extern unsigned char DAT_8009d618;
extern G DAT_8009d610;
extern unsigned short DAT_8009d670;
extern unsigned short DAT_8009d674;
extern unsigned short DAT_800a6114;
extern unsigned short DAT_1f8001fc;
extern unsigned short DAT_1f8001fe;

#define AXIS(v, hi, lo) ({                                  \
    unsigned char a = (v);                                      \
    unsigned short b;                                           \
    int r;                                                      \
    (v) = 0;                                                    \
    if ((unsigned)(a - 0x50) < 0x60) r = 0;                     \
    else {                                                      \
        b = a - 0x30;                                           \
        if (b < 0xa0) { (v) = 1; { int t = (b < 0x50) ? hi : lo; r = t; } }    \
        else {                                                  \
            b = a - 0x10;                                       \
            if (b < 0xe0) { (v) = 2; { int t = (b < 0x70) ? hi : lo; r = t; } } \
            else { (v) = 3; { int t = (a < 0x80) ? hi : lo; r = t; } }         \
        }                                                       \
    }                                                           \
    r; })

void FUN_80025d90(void)
{
    unsigned short m;
    volatile unsigned short *pad;
    G *g = &DAT_8009d610;
    DAT_8009d674 = DAT_800a6114;
    m = *(volatile unsigned short *)&DAT_8009d670 & 0xff0f;
    m |= AXIS(DAT_8009d618, 0x80, 0x20);
    m |= AXIS(g->b9, 0x10, 0x40);
    pad = &DAT_8009d670;
    *pad = m;
    DAT_1f8001fc = *pad & ~DAT_8009d674;
    DAT_1f8001fe = DAT_8009d674 & ~*pad;
    DAT_800a6114 = *pad;
}
