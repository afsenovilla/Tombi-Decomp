// FUNC 8011e524 2460 X001
// MATCHING 8011e524 2460
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x14 - 8];
    short yf, yw;
    char p18[0x34 - 0x18];
    int d34;
    char p38[0x68 - 0x38];
    unsigned char b68, b69, b6a, b6b;
    char p6c[0x70 - 0x6c];
    short box2;
    char p72[0x7e - 0x72];
    short velY;
    char p80[0x82 - 0x80];
    unsigned short w82;
    char p84[0x9c - 0x84];
    unsigned char b9c;
    char p9d[0xa0 - 0x9d];
    int *da0;
    unsigned char ba4;
    unsigned char flags[0xb2 - 0xa5];
    unsigned short wb2;
} O;
typedef struct {
    char p0[0xa];
    short w0a;
    char p0c[0x12 - 0xc];
    short w12;
    char p14[0x1a - 0x14];
    short w1a;
    char p1c[0x22 - 0x1c];
    short w22;
    char p24[0x28 - 0x24];
} E;

extern O D_800A6038;
extern short D_1F8000EE;
extern void func_8011EEC0(O *);

#define DV ((short)o->w82 >> 8)

void func_8011E524(O *o, short n)
{
    int *q = o->da0;
    short cnt;
    E *e;
    unsigned char *f;

    q++;
    cnt = *q++;
    if (n < 3 || n >= cnt) return;
    if (D_1F8000EE >= 0x4b1 && o->b6a != 0) {
        f = o->flags;
        o->b6a = 0;
        o->step = 0;
        do {
            ((E *)q)->w0a = 0;
            ((E *)q)->w12 = 0;
            ((E *)q)->w1a = 5;
            ((E *)q)->w22 = 5;
            *f++ = 0;
        } while (--cnt);
    }
    e = (E *)q + n;
    o->b68 = 0;
    if ((unsigned short)(D_800A6038.yw + D_800A6038.box2 - (o->yw + e->w0a)) < 0x19 && D_800A6038.b9c != 1) {
        D_800A6038.yw = o->yw + e->w0a - D_800A6038.box2;
        D_800A6038.yf = 0;
        D_800A6038.velY = 0;
        D_800A6038.b69 = 1;
        o->b68 = 1;
        o->b6b = n;
    }
    switch (o->step) {
    case 0: {
        short v;
        if (o->b68 == 0) break;
        v = (short)D_800A6038.wb2 >> 8;
        if (v == 0) break;
        if (v > 0) {
            if (v < 2) o->w82 = 0x100;
            else if (v < 3) o->w82 = 0x200;
            else o->w82 = 0x280;
        } else {
            if (v >= -1) o->w82 = 0x100;
            else if (v >= -2) o->w82 = 0x180;
            else if (v >= -3) o->w82 = 0x200;
            else o->w82 = 0x280;
        }
        if (n < cnt >> 1) o->d34 = n * DV;
        else o->d34 = (cnt - n) * DV;
        o->b6a = 1;
        o->step++;
    }
    case 1:
        e = (E *)q + o->b6b;
        f = o->flags;
        f += o->b6b;
        switch (*f) {
        case 0:
            e->w0a += DV;
            e->w12 += DV;
            e->w1a += DV;
            e->w22 += DV;
            break;
        case 1:
            e->w0a = e->w12;
            e->w1a = e->w22;
            *f = 0;
            break;
        case 2:
            e->w12 = e->w0a;
            e->w22 = e->w1a;
            *f = 0;
            break;
        }
        if (e->w0a >= o->d34) o->step++;
        if (D_800A6038.b9c != 0) break;
        D_800A6038.yw = o->yw + e->w0a - D_800A6038.box2;
        break;
    case 2:
        e = (E *)q + o->b6b;
        e->w0a -= DV;
        e->w12 -= DV;
        e->w1a -= DV;
        e->w22 -= DV;
        if (D_800A6038.b9c == 0) D_800A6038.yw -= DV;
        if (e->w0a > 0) break;
        o->step++;
        break;
    case 3:
        if (o->b68) {
            if (D_800A6038.wb2 >> 8) o->step = 0;
            else o->step++;
        }
        if (D_800A6038.b9c) o->step = 8;
        break;
    case 4: {
        short a;
        n = o->b6b;
        o->w82 -= 0x40;
        a = DV;
        if (a == 0) {
            o->step = 7;
        } else {
            if (n < cnt >> 1) o->d34 = n * a;
            else o->d34 = (cnt - n) * a;
            o->step++;
        }
    }
    case 5:
        e = (E *)q + o->b6b;
        e->w0a += DV;
        e->w12 += DV;
        e->w1a += DV;
        e->w22 += DV;
        if (e->w0a >= o->d34) o->step++;
        if (D_800A6038.b9c) o->step += 4;
        else D_800A6038.yw += DV;
        if (D_800A6038.wb2 >> 8) o->step = 0;
        break;
    case 6:
        e = (E *)q + o->b6b;
        e->w0a -= DV;
        e->w12 -= DV;
        e->w1a -= DV;
        e->w22 -= DV;
        if (e->w0a <= 0) o->step -= 2;
        if (D_800A6038.b9c) o->step += 4;
        else D_800A6038.yw -= DV;
        if (D_800A6038.wb2 >> 8) o->step = 0;
        break;
    case 7:
        if (D_800A6038.wb2 >> 8) o->step = 0;
        if (D_800A6038.b9c) o->step = 0xb;
        break;
    case 8: {
        short a;
        n = o->b6b;
        o->w82 -= 0x40;
        a = DV;
        if (a == 0) {
            o->step = 0xb;
        } else {
            if (n < cnt >> 1) o->d34 = n * a;
            else o->d34 = (cnt - n) * a;
            o->step++;
        }
    }
    case 9:
        e = (E *)q + o->b6b;
        e->w0a += DV;
        e->w12 += DV;
        e->w1a += DV;
        e->w22 += DV;
        if (e->w0a >= o->d34) o->step++;
        if (D_800A6038.b9c) break;
        o->step -= 4;
        D_800A6038.yw += DV;
        break;
    case 10:
        e = (E *)q + o->b6b;
        e->w0a -= DV;
        e->w12 -= DV;
        e->w1a -= DV;
        e->w22 -= DV;
        if (e->w0a <= 0) o->step -= 2;
        if (D_800A6038.b9c) break;
        o->step -= 4;
        D_800A6038.yw -= DV;
        break;
    case 11:
        if (o->b68 == 0) break;
        o->w82 = 0x100;
        if (n < cnt >> 1) o->d34 = n;
        else o->d34 = cnt - n;
        o->step = 1;
        break;
    }
    func_8011EEC0(o);
}
