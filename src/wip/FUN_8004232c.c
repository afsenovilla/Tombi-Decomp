// FUNC 8004232c 304 MAIN0
extern short *FUN_8003f200(int, int);
extern unsigned short *DAT_1f800278;

int FUN_8004232c(short a, int b, short c)
{
    char pad[8];
    short *p;
    unsigned short *q;
    unsigned short fl, x, w;
    int n, cnt, d;
    p = FUN_8003f200(a, c);
    DAT_1f800278 = (unsigned short *)(p + 1);
    n = cnt = *p;
    goto test;
    top:
    {
        cnt--;
        q = DAT_1f800278;
        DAT_1f800278 = q + 1;

        if (*q & 0x4000) {
            if (*q & 0x10) goto hit;
        }
        DAT_1f800278 = q + 4;
        n = cnt << 16;
        goto test;
        hit:
        DAT_1f800278 = q + 2;
        x = q[1];
        DAT_1f800278 = q + 3;
        w = q[2];
        DAT_1f800278 = q + 4;
        d = b - x - w;
        if ((int)((d - 1) & 0xffff) <= -(short)w) {
            if ((int)((b - (x + 8) - w - 1) & 0xffff) <= -(short)w) return 1;
            return 2;
        }
        if (d << 16 > 0) return 0;
        n = cnt << 16;
    }
    test:
    if (n != 0) goto top;
    return 0;
}
