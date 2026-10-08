// FUNC 8011ec68 1384 X000
// MATCHING 8011ec68 1384
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef union { int raw; struct { unsigned short frac; short whole; } p; } FX;
typedef struct S {
    unsigned char active, visible, p02[2], b04, p05[7], b0c, p0d[2], b0f;
    FX a, y, b;
    char p1c[0x2e - 0x1c];
    unsigned short animFrame;
    char p30[0x68 - 0x30];
    unsigned char b68, b69, p6a[2];
    short box0, box1, box2, box3;
    char p74[0x84 - 0x74];
    int d84, d88, d8c, d90;
    struct S *next;
    char p98[0xb4 - 0x98];
    unsigned short wb4, wb6;
    unsigned short *pb8, *pbc;
    unsigned short wc0, pc2;
    int c4, c8, cc;
} S;
extern unsigned short *DAT_80138d60[];
extern unsigned short *DAT_80138d54[];
extern unsigned short DAT_80138d0c[];
extern int DAT_8009c984;
extern int DAT_8009c984a[];
extern int DAT_8009c984b[];
extern unsigned char DAT_8009c938a[];
extern unsigned char DAT_8009c938;
extern int FUN_8001f9e0(void);
extern void FUN_8011a148(int, int, int, int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8001e4f0(int);
extern void FUN_8006424c(SVECTOR *, MATRIX *);
extern void FUN_80063a6c(MATRIX *, VECTOR *, VECTOR *);
extern void FUN_800202b4(S *);
extern void FUN_80018838(S *);

void FUN_8011ec68(S *o)
{
    S *q, *p;
    int a, y, b;
    int flag;
    unsigned short **pp;
    VECTOR in, out;
    SVECTOR v;
    MATRIX m;
    char pad[16];

    switch (o->b04) {
    case 0:
        if (o->d90 == 0) {
            q = o;
            p = o;
            a = o->a.raw;
            y = o->y.raw;
            b = o->b.raw;
            for (;;) {
                q->wb4 = 0;
                q->wb6 = 0;
                q->pb8 = DAT_80138d60[q->b0c];
                q->pbc = DAT_80138d60[q->b0c];
                switch (q->b0c) {
                case 1:
                    q->box0 = 0x10;
                    q->box1 = 0x20;
                    q->box2 = 0x1e;
                    q->box3 = 100;
                    q->wc0 = 0;
                    q->b0f = 0;
                    if (!(DAT_8009c984a[0] & 1))
                        FUN_8011a148(q->a.p.whole, q->y.p.whole, q->b.p.whole, 0);
                    break;
                case 0:
                    q->active = 2;
                    q->b0f = 0;
                    break;
                case 2:
                    q->active = 2;
                    *(signed char *)&q->b0f = -1;
                    break;
                }
                q = q->next;
                if (q == 0)
                    break;
                p->c4 = q->a.raw - a;
                p->c8 = q->y.raw - y;
                p->cc = q->b.raw - b;
                a = q->a.raw;
                y = q->y.raw;
                b = q->b.raw;
                p = q;
            }
        }
        o->b04++;
        break;
    case 1:
        if (o->d90 == 0) {
            for (q = o; ; q = q->next) {
                if (q->b0c == 1) {
                    flag = 0;
                    if (!(DAT_8009c984a[0] & 1) && (FUN_8001f9e0() & 0x3f) == 0) {
                        o->wb4 = 2;
                        flag = 0; if (!(DAT_8009c984a[0] & 1)) flag = 1;
                    }
                    if (q->b69 == 1 && o->wb6 == 0) {
                        o->wb4 = 1;
                        o->wb6 = 4;
                        if (!(DAT_8009c984b[0] & 1))
                            flag = 1;
                        q->wc0 = 0x28;
                    }
                    if (q->b68 == 1 && o->wb6 == 0) {
                        o->wb4 = 1;
                        o->wb6 = 4;
                        if (!(DAT_8009c984b[0] & 1))
                            flag = 1;
                        q->wc0 = 0x28;
                    }
                    q->b69 = 0;
                    q->b68 = 0;
                    o->animFrame = q->animFrame;
                    if (DAT_8009c938a[0] == 0 && q->wc0 != 0 && --q->wc0 == 0 && !(DAT_8009c984a[0] & 1)) {
                        DAT_8009c984a[0] |= 1;
                        FUN_8005a8a8(2, 0, 0);
                        FUN_8001e4f0(0x35);
                    }
                    break;
                }
            }
            q = o;
            a = o->a.raw;
            y = o->y.raw;
            b = o->b.raw;
            for (;;) {
                pp = &q->pb8;
                if (flag) {
                    q->wb4 = o->wb4;
                    switch (o->wb4) {
                    case 1:
                        q->pb8 = DAT_80138d54[q->b0c];
                        break;
                    case 2:
                        q->pbc = DAT_80138d0c;
                        break;
                    }
                }
                v.vx = *pp[0];
                v.vy = 0;
                v.vz = *pp[1];
                FUN_8006424c(&v, &m);
                q->d84 = v.vx;
                q->d88 = v.vy;
                q->d8c = v.vz;
                if (*++pp[0] == 0x8000) {
                    q->wb4 = 0;
                    pp[0] = DAT_80138d60[q->b0c];
                }
                if (*++pp[1] == 0x8000)
                    pp[1] = DAT_80138d60[q->b0c];
                in.vx = q->c4;
                in.vy = q->c8;
                in.vz = q->cc;
                FUN_80063a6c(&m, &in, &out);
                a += out.vx;
                y += out.vy;
                b += out.vz;
                if (q->next == 0)
                    break;
                q = q->next;
                q->a.raw = a;
                q->y.raw = y;
                q->b.raw = b;
            }
        }
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
