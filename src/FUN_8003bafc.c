// FUNC 8003bafc 396 MAIN0
// MATCHING 8003bafc 396
extern int D_1f800070;
extern int D_1f800074;
extern unsigned char DAT_8009bd60;
extern unsigned short DAT_8009bd64;
extern void FUN_8005bd24(unsigned char *, unsigned char *, int);

static __inline__ short step(void)
{
    if (++DAT_8009bd60 < 16) return 0;
    return 1;
}

void FUN_8003bafc(unsigned char *p, unsigned char *dst)
{
    unsigned char n;
    unsigned char off;

    p += 4;
    D_1f800070 = ((p[1] << 8) | p[0]) | ((p[3] << 8) | (p[2] << 16));
    p += 4;
    DAT_8009bd64 = (p[1] << 8) | p[0];
    p += 2;
    DAT_8009bd60 = 0;
    D_1f800074 = 0;
    do {
        if ((DAT_8009bd64 >> DAT_8009bd60) & 1) {
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
            DAT_8009bd64 = (p[1] << 8) | p[0];
            p += 2;
            DAT_8009bd60 = 0;
        }
    } while (D_1f800070 > D_1f800074);
}
