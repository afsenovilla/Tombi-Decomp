// FUNC 8004232c 304 MAIN0
// wip: solo falta intercambiar registros: cnt sale en a3 y x en t0 (juego: cnt=t0, x=a3).
extern short *FUN_8003f200(int, int);
extern unsigned short *DAT_1f800278;

int FUN_8004232c(short a, int b, short c)
{
    char pad[8];
    short *p;
    unsigned short *q;
    unsigned short f;
    int n, cnt, x, w, d, e;
    p = FUN_8003f200(a, c);
    DAT_1f800278 = (unsigned short *)(p + 1);
    n = cnt = *p;
    goto test;
top:
    q = DAT_1f800278;
    DAT_1f800278 = q + 1;
    f = *q;
    cnt--;
    if (f & 0x4000) {
        if (f & 0x10) goto hit;
    }
    DAT_1f800278 = q + 4;
    goto next;
hit:
    x = *DAT_1f800278++;
    w = *DAT_1f800278++;
    DAT_1f800278++;
    d = b - x - w;
    if (((d - 1) & 0xffff) > -(short)w) {
        if (d << 16 > 0) return 0;
        goto next;
    }
    e = x + 8;
    if (((b - e - w - 1) & 0xffff) > -(short)w) return 2;
    return 1;
next:
    n = cnt << 16;
test:
    if (n != 0) goto top;
    return 0;
}
