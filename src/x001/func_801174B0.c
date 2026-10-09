// FUNC 801174b0 1828 X001
// MATCHING 801174b0 1828
typedef struct { char p[3]; unsigned char n; int e[1]; } Q;

extern unsigned short D_8009C962;
extern unsigned char D_8009CDA2;
extern int D_800A4574, D_800A4570;
extern short D_1F800176, D_1F80017A, D_1F8000E6, D_1F80016A;
extern int getScrollOffsetX(void);

void func_801174B0(Q *q, unsigned char *base)
{
    short lo, hi, a, b, i, t;
    int cnt = base[0];
    int *off = (int *)(base + 4);

    switch (D_8009C962) {
    case 0:
        if (D_8009CDA2) {
            lo = (D_1F800176 + 0x50) / 80;
            hi = lo + 8;
        } else if (D_800A4574 == 0) {
            lo = (D_1F800176 + 0x78) / 80;
            hi = lo + 9;
        } else {
            lo = (D_1F800176 + getScrollOffsetX() - 0x28) / 80;
            hi = lo + 0xc;
        }
        break;
    case 1:
        if (D_8009CDA2) {
            lo = (D_1F800176 - 0x50) / 80;
            hi = lo + 8;
        } else if (D_800A4574 == 0) {
            lo = (D_1F800176 + 0x64) / 80;
            hi = lo + 9;
        } else {
            lo = (D_1F800176 + getScrollOffsetX() + 0x64) / 80;
            hi = lo + 0xa;
        }
        break;
    case 2:
        if (D_800A4574 == 0) {
            lo = (D_1F800176 - 0x78) / 80;
            hi = lo + 0x11;
            if (lo < 0) lo = 0;
            if (hi >= 0x21) hi = 0x20;
            t = (D_1F800176 - 0x50) / 80;
            a = t + 0x21;
            b = t + 0x2c;
            if (a < 0x21) a = 0x21;
            if (b >= 0x3f) b = 0x3e;
            for (i = a; i <= b; i++) {
                q->e[q->n] = (int)base + off[i];
                q->n++;
            }
        } else {
            short *px = &D_1F800176;
            { int u = getScrollOffsetX() - 0x118;
            lo = (*px + u) / 80; }
            hi = lo + 0x12;
            if (lo < 0) lo = 0;
            if (hi >= 0x21) hi = 0x20;
            t = (*px + getScrollOffsetX() - 0xa0) / 80;
            a = t + 0x21;
            b = t + 0x2f;
            if (a < 0x21) a = 0x21;
            if (b >= 0x3f) b = 0x3e;
            for (i = a; i <= b; i++) {
                q->e[q->n] = (int)base + off[i];
                q->n++;
            }
        }
        if (D_1F80017A + D_1F8000E6 < -0x59) {
            if (D_800A4570 >= 0) break;
            if (D_1F80017A < -0x77) break;
        }
        t = (D_1F800176 - 0xdc) / 80;
        a = t + 0x3f;
        b = t + 0x46;
        if (a < 0x3f) a = 0x3f;
        if (b >= cnt) b = cnt - 1;
        for (i = a; i <= b; i++) {
            q->e[q->n] = (int)base + off[i];
            q->n++;
        }
        break;
    case 3:
        if (D_8009CDA2) {
            lo = D_1F800176 / 80;
            hi = lo + 9;
        } else if (D_800A4574 == 0) {
            lo = D_1F800176 / 80;
            hi = lo + 9;
        } else {
            lo = (D_1F800176 + getScrollOffsetX()) / 80;
            hi = lo + 9;
        }
        break;
    case 4:
        if (D_8009CDA2) {
            if (D_1F80016A >= 0xf3d) {
                lo = (D_1F800176 - 0x5dc) / 80;
                hi = lo + 8;
            } else {
                hi = (D_1F800176 - 0x384) / 80;
                lo = hi - 0x12;
            }
        } else if (D_800A4574 == 0) {
            lo = (D_1F800176 - 0x636) / 80;
            hi = lo + 9;
        } else {
            lo = (D_1F800176 + getScrollOffsetX() - 0x65e) / 80;
            hi = lo + 0xa;
        }
        break;
    }
    if (lo < 0) lo = 0;
    if (hi >= cnt) hi = cnt - 1;
    for (i = lo; i <= hi; i++) {
        q->e[q->n] = (int)base + off[i];
        q->n++;
    }
}
