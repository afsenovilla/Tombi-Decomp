// FUNC 8011bf68 2204 X000
// MATCHING 8011bf68 2204
/* Real size 2204 B: splat cut it at 2052 and listed the tail as func_8011C758 (172 B), which is part of this function. */
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { long vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { unsigned short frac; short whole; } FixParts;
typedef union { int raw; FixParts p; } Fix16;

typedef struct E {
    unsigned char active;
    char p01[2];
    unsigned char subtype;
    unsigned char b04;
    char p05[0x10 - 5];
    Fix16 a, y, b;
    char p1c[0x20 - 0x1c];
    short timer;
    char p22[0x28 - 0x22];
    void *movetab;
    unsigned short animTimer;
    unsigned short animFrame;
    int d30, d34, d38;
    char p3c[0x68 - 0x3c];
    unsigned char b68, b69;
    char p6a[2];
    short box0, box1, box2, box3;
    char p74[0x84 - 0x74];
    int d84, d88, d8c;
    int d90;
    struct E *next;
    char p98[0xa4 - 0x98];
    unsigned char ba4;
    char pa5[0xb4 - 0xa5];
    unsigned short wb4;
    unsigned short wb6;
    unsigned short wb8, wba, wbc, wbe;
    signed char *t0;
    signed char *t1;
    unsigned short c8, ca, cc, ce, d0, d2;
} E;

extern char D_80077CDC[];
extern signed char D_8013882C[];
extern signed char D_80138830[];
extern signed char D_80138838[];
extern signed char D_80138840[];
extern unsigned char D_800A60D6;
extern void FUN_801191b0(short, short, short, short);
extern void FUN_8001e4f0(int);
extern short FUN_8001fe0c(short, int);
extern void FUN_8006424c(SVECTOR *, MATRIX *);
extern void FUN_80063a6c(MATRIX *, VECTOR *, VECTOR *);
extern void FUN_800202b4(E *);
extern void FUN_80018838(E *);

void func_8011BF68(E *o)
{
    E *e;
    E *n;
    int x, y, z;
    unsigned short i;
    unsigned short af;
    int flag;
    signed char *t0;
    signed char *t1;
    signed char **tp;
    MATRIX m;
    VECTOR in;
    VECTOR out;
    SVECTOR v;

    switch (o->b04) {
    case 0:
        if (o->d90 == 0) {
            e = o;
            n = o;
            i = 0;
            x = o->a.raw;
            y = o->y.raw;
            z = o->b.raw;
            o->timer = 0;
            o->d30 = 0;
            o->d34 = 0;
            o->d38 = 0;
            o->d0 = 0;
            for (;;) {
                e->animTimer = i;
                e->movetab = D_80077CDC;
                e->b68 = 0;
                e->b69 = 0;
                e->wb4 = 0;
                e->wb6 = 0;
                e->wb8 = 0;
                e->wba = 0;
                e->wbc = 0x80;
                e->wbe = 0x80;
                e->t0 = D_80138830 + D_8013882C[o->subtype];
                e->t1 = D_80138830 + D_8013882C[o->subtype];
                i++;
                e->c8 = 0;
                e->ca = 0;
                e->cc = 0;
                e->ce = 0;
                e->d84 = 0x1000;
                e->d88 = 0x1000;
                e->d8c = 0x1000;
                e->ba4 = 0;
                if (e->next == 0) {
                    e->box1 = 0x20;
                    e->box0 = 0x10;
                    e->box2 = 0x10;
                    e->box3 = 0x1e;
                } else {
                    e->active = 2;
                }
                e = e->next;
                if (e == 0) break;
                n->d30 = e->a.raw - x;
                n->d34 = e->y.raw - y;
                n->d38 = e->b.raw - z;
                x = e->a.raw;
                y = e->y.raw;
                z = e->b.raw;
                n = e;
            }
        }
        o->b04++;
        break;
    case 1:
        if (o->d90 != 0) goto tail;
        for (e = o; ; e = e->next) {
            if (e->next == 0) {
                o->animFrame = af = e->animFrame;
                flag = 0;
                switch (e->b69) {
                case 1:
                    if (o->wb6 == 0) {
                        flag = 1;
                        switch (af) {
                        case 0: o->wb4 = 1; break;
                        case 1: o->wb4 = 5; break;
                        case 2: o->wb4 = 2; break;
                        case 3: o->wb4 = 6; break;
                        }
                        o->wb6 = 4;
                    }
                    e->b69 = 0;
                    break;
                case 4:
                    if (o->wb6 == 0) {
                        flag = 1;
                        if (af == 4) o->wb4 = 3;
                        else o->wb4 = 4;
                        o->wb6 = 4;
                    }
                    e->b69 = 0;
                    break;
                default:
                    if (o->wb6 != 0) o->wb6--;
                    break;
                }
                switch (e->b68) {
                case 1:
                    o->wb4 = 2;
                    flag = 1;
                    o->animFrame = e->b68;
                    e->b68 = 0;
                    break;
                case 2:
                    if (o->d0 != 4 && D_800A60D6 == 4) {
                        flag = 2;
                        switch (o->animFrame) {
                        case 0: o->wb4 = 1; break;
                        case 1: o->wb4 = 5; break;
                        case 2: o->wb4 = 2; break;
                        case 3: o->wb4 = 6; break;
                        }
                    }
                    e->b68 = 0;
                    break;
                }
                o->d0 = e->b69;
                break;
            }
        }
        e = o;
        x = o->a.raw;
        y = o->y.raw;
        z = o->b.raw;
        t0 = o->t0;
        t1 = o->t1;
        tp = &o->t0;
        if (flag) {
            switch (o->wb4 = o->wb4) {
            case 5:
                o->ce = (0x80 - o->ce) & 0xff;
            case 1:
                tp[0] = D_80138838 + D_8013882C[o->subtype];
                tp[1] = D_80138840 + D_8013882C[o->subtype];
                o->wb8 = 9;
                o->wba = 9;
                o->wbc = 0x80;
                o->wbe = 0x80;
                break;
            case 6:
                o->ce = (0x80 - o->ce) & 0xff;
            case 2:
                tp[0] = D_80138838 + D_8013882C[o->subtype];
                tp[1] = D_80138840 + D_8013882C[o->subtype];
                o->wb8 = 0x11;
                o->wba = 9;
                o->wbc = 0x80;
                o->wbe = 0x40;
                o->cc = (0x80 - o->cc) & 0xff;
                break;
            case 3:
                o->ce = (0x80 - o->ce) & 0xff;
            case 4:
                tp[0] = D_80138838 + D_8013882C[o->subtype];
                tp[1] = D_80138840 + D_8013882C[o->subtype];
                o->wb8 = 9;
                o->wba = 9;
                o->wbc = 0x20;
                o->wbe = 0x20;
                break;
            }
        }
        for (;;) {
            if (flag == 2 && e->next == 0 && o->timer == 0) {
                FUN_801191b0(o->subtype, e->a.p.whole + 0x10, e->y.p.whole + 0x20, e->b.p.whole);
                FUN_8001e4f0(0x32);
                o->timer = 0x78;
            }
            v.vx = FUN_8001fe0c(o->cc, 0xf) * (o->c8 >> 2) / *t0;
            v.vy = 0;
            v.vz = FUN_8001fe0c(o->ce, 0xf) * (o->ca >> 2) / *t1;
            FUN_8006424c(&v, &m);
            t0++;
            t1++;
            e->d84 = v.vx;
            e->d88 = v.vy;
            e->d8c = v.vz;
            in.vx = e->d30;
            in.vy = e->d34;
            in.vz = e->d38;
            FUN_80063a6c(&m, &in, &out);
            x += out.vx;
            y += out.vy;
            z += out.vz;
            if (e->next == 0) break;
            e = e->next;
            e->a.raw = x;
            e->y.raw = y;
            e->b.raw = z;
        }
        o->c8 += o->wb8;
        o->ca += o->wba;
        if (o->c8 > o->wbc) o->wb8 = 0;
        if (o->ca > o->wbe) o->wba = 0;
        if (o->c8 != 0) o->c8--;
        else if ((short)o->c8 < 0) o->c8++;
        if (o->ca != 0) o->ca--;
        else if ((short)o->ca < 0) o->ca++;
        o->cc = (o->cc - 4) & 0xff;
        if (o->wb4 == 3) o->ce = o->ce + 2;
        else o->ce = o->ce - 2;
        o->ce = *(unsigned char *)&o->ce;
        if (o->c8 == 0 && o->d2 == 0 && o->ca == 0) {
            o->cc = 0;
            o->ce = 0;
        }
    tail:
        if (o->timer != 0) o->timer--;
        FUN_800202b4(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
