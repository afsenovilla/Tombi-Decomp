// FUNC 80116764 464 X004
// MATCHING 80116764 464

typedef struct {
    unsigned char pad00[7];
    unsigned char substep; /* 0x07 */
    unsigned char pad08[0x18];
    int d20;               /* 0x20 */
    int d24;               /* 0x24 */
    unsigned char pad28[0x17];
    unsigned char b3f;     /* 0x3f */
    unsigned char pad40[0xa];
    short w4a;             /* 0x4a */
    unsigned char pad4c[0xe];
    short w5a;             /* 0x5a */
    unsigned char pad5c[2];
    short w5e;             /* 0x5e */
    short w60;             /* 0x60 */
    short w62;             /* 0x62 */
} S;

int func_80116764(S *o)
{
    int r = 0;

    switch (o->substep) {
    case 0:
        o->w62 = 0;
        o->w5e = 0;
        o->w60 = 0;
        o->w4a = 0;
        o->w5a = 0;
        o->substep++;
        break;
    case 1:
        o->w60 = 0x100;
        if (o->b3f == 0) {
            o->w5e += 0x100;
            if (o->w5e >= 0x5a00) {
                o->w5e = 0x5a00;
                o->w5a++;
            }
            if (o->w5e <= 0x2d00) {
                o->w62 += 0x80;
                if (o->w62 > 0x1200) o->w62 = 0x1200;
            } else if (o->w5e >= 0x4600) {
                goto dec;
            }
        } else {
            o->w5e -= 0x100;
            if (o->w5e <= -0x5a00) {
                o->w5e = -0x5a00;
                o->w5a++;
            }
            if (o->w5e >= -0x2d00) {
                o->w62 += 0x80;
                if (o->w62 > 0x1200) o->w62 = 0x1200;
            } else if (o->w5e < -0x1bff) {
            dec:
                o->w62 -= 0x80;
                if (o->w62 <= 0xa00) o->w62 = 0xa00;
            }
        }
        o->d24 = o->w5e;
        o->d20 = o->w62;
        if (o->w5a != 0) o->substep++;
        break;
    case 2:
        r = 1;
        break;
    }
    return r;
}
