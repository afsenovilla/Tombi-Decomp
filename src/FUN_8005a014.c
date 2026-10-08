// FUNC 8005a014 1932 MAIN0
// MATCHING 8005a014 1932
#define U8(o, k) (*(unsigned char *)((char *)(o) + (k)))
#define S8(o, k) (*(signed char *)((char *)(o) + (k)))
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
extern char *D_8009C338;
extern int MulCos(int, short);
extern int MulNegSinScaled(int, short);

static __inline__ void box(short *p, int a, int len)
{
    short b;
    int cy;

    b = (a + 0x140) & 0xff;
    p[0] = MulCos(b, 8);
    p[1] = MulNegSinScaled(b, 8);
    b = (a + 0xc0) & 0xff;
    p[2] = MulCos(b, 8);
    p[3] = MulNegSinScaled(b, 8);
    b = MulCos(a, len);
    cy = MulNegSinScaled(a, len);
    p[4] = p[0] + b;
    p[6] = p[2] + b;
    p[5] = p[1] + cy;
    p[7] = p[3] + cy;
}

void FUN_8005a014(char *o, short dx, short dy)
{
    char *p = D_8009C338;
    char *r;
    char *q1;
    char *q2;
    char *q3;
    char *q4;
    char *q5;
    char *q6;
    short a;

    switch (S8(p, 0xc)) {
    case 1:
        a = (U16(o, 0xaa) + 0x80) & 0xff;
        U16(p, 0x3c) = a;
        box((short *)(p + 0x10), a, U16(p, 0x30));
        q1 = D_8009C338;
        S16(q1, 0x34) = dx;
        S16(q1, 0x36) = dy;
        S16(q1, 0x10) += dx;
        S16(q1, 0x14) += dx;
        S16(q1, 0x18) += dx;
        S16(q1, 0x1c) += dx;
        S16(q1, 0x12) += dy;
        S16(q1, 0x16) += dy;
        S16(q1, 0x1a) += dy;
        S16(q1, 0x1e) += dy;
        break;
    case 2:
        a = (U16(o, 0xaa) + 0x80) & 0xff;
        U16(p, 0x3e) = a;
        box((short *)(p + 0x20), a, U16(p, 0x32));
        q2 = D_8009C338;
        a = U8(q2, 0x3c);
        S16(q2, 0x38) = dx;
        S16(q2, 0x3a) = dy;
        S16(q2, 0x20) += dx;
        S16(q2, 0x24) += dx;
        S16(q2, 0x28) += dx;
        S16(q2, 0x2c) += dx;
        S16(q2, 0x22) += dy;
        S16(q2, 0x26) += dy;
        S16(q2, 0x2a) += dy;
        S16(q2, 0x2e) += dy;
        box((short *)(q2 + 0x10), a, U16(q2, 0x30));
        q3 = D_8009C338;
        S16(q3, 0x10) += S16(q3, 0x34);
        S16(q3, 0x14) += S16(q3, 0x34);
        S16(q3, 0x18) += S16(q3, 0x34);
        S16(q3, 0x1c) += S16(q3, 0x34);
        S16(q3, 0x12) += S16(q3, 0x36);
        S16(q3, 0x16) += S16(q3, 0x36);
        S16(q3, 0x1a) += S16(q3, 0x36);
        S16(q3, 0x1e) += S16(q3, 0x36);
        break;
    case 3:
        a = (U16(o, 0xaa) + 0x80) & 0xff;
        U16(p, 0x3c) = a;
        box((short *)(p + 0x10), a, U16(p, 0x30));
        q4 = D_8009C338;
        a = U8(q4, 0x3e);
        S16(q4, 0x34) = dx;
        S16(q4, 0x36) = dy;
        S16(q4, 0x10) += dx;
        S16(q4, 0x14) += dx;
        S16(q4, 0x18) += dx;
        S16(q4, 0x1c) += dx;
        S16(q4, 0x12) += dy;
        S16(q4, 0x16) += dy;
        S16(q4, 0x1a) += dy;
        S16(q4, 0x1e) += dy;
        box((short *)(q4 + 0x20), a, U16(q4, 0x32));
        q5 = D_8009C338;
        S16(q5, 0x20) += S16(q5, 0x38);
        S16(q5, 0x24) += S16(q5, 0x38);
        S16(q5, 0x28) += S16(q5, 0x38);
        S16(q5, 0x2c) += S16(q5, 0x38);
        S16(q5, 0x22) += S16(q5, 0x3a);
        S16(q5, 0x26) += S16(q5, 0x3a);
        S16(q5, 0x2a) += S16(q5, 0x3a);
        S16(q5, 0x2e) += S16(q5, 0x3a);
        break;
    default:
        a = (U16(o, 0xaa) + 0x80) & 0xff;
        r = D_8009C338;
        U16(r, 0x3c) = a;
        box((short *)(r + 0x10), a, U16(r, 0x30));
        q6 = D_8009C338;
        S16(q6, 0x10) = S16(q6, 0x14) = S16(q6, 0x18) = S16(q6, 0x1c) = 0;
        S16(q6, 0x12) = S16(q6, 0x16) = S16(q6, 0x1a) = S16(q6, 0x1e) = 0;
        S16(q6, 0x20) = S16(q6, 0x24) = S16(q6, 0x28) = S16(q6, 0x2c) = 0;
        S16(q6, 0x22) = S16(q6, 0x26) = S16(q6, 0x2a) = S16(q6, 0x2e) = 0;
        break;
    }
}
