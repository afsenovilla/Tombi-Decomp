// FUNC 8003bafc 396 MAIN0
// MATCHING 8003bafc 396
#include "globals.h"
extern int LZ_FILE_CTRL;
extern int D_1f800074;
extern void FUN_8005bd24(unsigned char *, unsigned char *, int);

static __inline__ short step(void)
{
    if (++LZ_CURRENT_BIT < 16) return 0;
    return 1;
}

void lzDecompress(unsigned char *p, unsigned char *dst)
{
    unsigned char n;
    unsigned char off;

    p += 4;
    LZ_FILE_CTRL = ((p[1] << 8) | p[0]) | ((p[3] << 8) | (p[2] << 16));
    p += 4;
    LZ_BITMASK = (p[1] << 8) | p[0];
    p += 2;
    LZ_CURRENT_BIT = 0;
    D_1f800074 = 0;
    do {
        if ((LZ_BITMASK >> LZ_CURRENT_BIT) & 1) {
            off = *p++;
            n = *p++;
            FUN_8005bd24(dst - off, dst, n);
            dst += n;
            D_1f800074 += n;
        } else {
            *dst++ = *p++;
            D_1f800074++;
        }
        if (step()) {
            LZ_BITMASK = (p[1] << 8) | p[0];
            p += 2;
            LZ_CURRENT_BIT = 0;
        }
    } while (LZ_FILE_CTRL > D_1f800074);
}
