// FUNC 800285ec 360 MAIN0
extern unsigned char DAT_800a60d6;
extern int DAT_800a606c;

void FUN_800285ec(int p)
{
    char pad[8];
    int d, n;
    int s;
    short e;
    unsigned short u;
    unsigned short *q = (unsigned short *)0x1f8000e6;
    unsigned short *r;
    if (DAT_800a60d6 == 3) {
        u = *(unsigned short *)0x1f8000f2;
        d = u - 0x14;
        d = d - DAT_800a606c;
    } else
        d = *(unsigned short *)0x1f8000f2 - *(unsigned short *)0x1f80016e;
    s = d;
    n = (short)(s + *q);
    if (n != -0x50) {
        if (n < -0x4f) {
            e = *q + 2;
            *q = e;
            if (s + e > -0x50)
                *q = -s - 0x50;
        } else {
            u = *q - 2;
            if (n > -0x34) {
                e = *(unsigned short *)0x1f8000f2 - 2;
                *(short *)0x1f8000f2 = *(short *)(p + 0x30);
                u = *(unsigned short *)0x1f8000e6;
                if (*(short *)0x1f8000f2 <= e) {
                    s = s - 2;
                    *(short *)0x1f8000f2 = e;
                }
            }
            r = (unsigned short *)0x1f8000e6;
            *q = u;
            if ((short)s + *(short *)r < -0x50)
                *r = -s - 0x50;
        }
    }
    if (*(short *)0x1f8000f2 + *(short *)0x1f8000e6 < *(short *)(p + 0x30))
        *(short *)0x1f8000e6 = *(short *)(p + 0x30) - *(short *)0x1f8000f2;
}
