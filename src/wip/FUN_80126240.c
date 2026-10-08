// FUNC 80126240 548 X000
#define U(p, o) (*(unsigned short *)((p) + (o)))
#define S(p, o) (*(short *)((p) + (o)))
#define P(p, o) (*(unsigned char **)((p) + (o)))

void FUN_80126240(unsigned char *a, unsigned char *b)
{
    int dx, dy, w, h, px, py, ex, ey, ax, ay;
    if (a[0x9e] == 5)
        return;
    if ((unsigned short)(U(P(a, 0x44), 2) - U(P(b, 0x44), 2) + 0x2d) >= 0x5b)
        return;
    w = U(b, 0x6c) + U(a, 0x6c);
    dx = U(P(a, 0x40), 2) - U(P(b, 0x40), 2);
    if ((unsigned short)(dx + w) > S(b, 0x6e) + S(a, 0x6e))
        return;
    dy = U(a, 0x16) - U(b, 0x16);
    h = U(b, 0x70) + U(a, 0x70);
    if ((unsigned short)(dy + h) > S(a, 0x72) + S(b, 0x72))
        return;
    if ((short)dx < 0) {
        ex = w;
        ax = -dx;
        px = -w;
    } else {
        px = (S(b, 0x6e) - U(b, 0x6c)) + (S(a, 0x6e) - U(a, 0x6c));
        ex = px;
        ax = dx;
    }
    if ((short)dy < 0) {
        ey = h;
        ay = -dy;
        py = -h;
    } else {
        py = (U(b, 0x72) - U(b, 0x70)) + (U(a, 0x72) - U(a, 0x70));
        ey = py;
        ay = dy;
    }
    if ((short)ex - (short)ax < (short)ey - (short)ay) {
        U(P(a, 0x40), 2) = U(P(b, 0x40), 2) + px;
        if ((short)px < 0)
            a[0xa6] = 2;
        else
            a[0xa6] = 3;
    } else if ((short)py > 0) {
        S(a, 0x16) = U(b, 0x16) + py;
        if (S(a, 0x7e) < 0)
            S(a, 0x7e) = 0;
    } else if (a[0x9c] & 1) {
        U(P(a, 0x40), 2) = U(P(b, 0x40), 2) + px;
        a[0xa6] = 2;
    } else {
        a[0xbe] = 9;
        S(a, 0x2e) = 1;
        S(a, 0x14) = 0;
        a[0x69] = 1;
        S(a, 0xb0) = -2;
        S(a, 0x16) = U(b, 0x16) + py;
        b[0x69] = 1;
    }
}
