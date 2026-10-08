// FUNC 8011b8c0 1704 X000
// MATCHING 8011b8c0 1704
#include "TOBJ.H"
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { long vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct {
    TObj t;
    int dc0;
    int dc4, dc8, dcc;
} TO2;
typedef struct { short x, y; } XY;
extern unsigned char *D_801387D4[];
extern XY D_80138814[];
extern signed char D_80138824[];
extern signed char D_801387E4[][12];
extern signed char D_80138718[][12];
extern signed char D_80138748[][12];
extern signed char D_80138720[];
extern signed char D_801386B4[];
extern short D_8007A3F0[];
extern unsigned char *FUN_800183b8(void);
extern void FUN_8001e4f0(int);
extern void FUN_8006424c(SVECTOR *, MATRIX *);
extern void FUN_80063a6c(MATRIX *, VECTOR *, VECTOR *);
extern void ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);

void FUN_8011b8c0(TO2 *o)
{
    VECTOR v;
    VECTOR out;
    SVECTOR r;
    MATRIX m;
    TO2 *p;
    unsigned char *q;
    unsigned char *tab;
    int x, y, z;
    int n;
    signed char f;
    short w;
    unsigned char b;
    Fix16 fx;
    TO2 *nx;
    int c;
    unsigned short t;
    unsigned short t1;

    switch (o->t.b04) {
    case 0:
        if (o->t.d90 == 0) {
            p = o;
            n = 0;
            x = o->t.a.raw;
            y = o->t.y.raw;
            z = o->t.b.raw;
            o->t.wbc = 0;
            *(unsigned short *)&o->t.bbe = 0;
            o->dc4 = 0;
            o->dc8 = 0;
            o->dcc = 0;
            tab = D_801387D4[o->t.subtype];
            for (;;) {
                p->t.animTimer = n++;
                p->t.movetab = o;
                p->t.b68 = 0;
                p->t.b69 = 0;
                p->t.wb4 = 0;
                p->t.wb8 = 0x69;
                p->t.box0 = *tab++;
                p->t.box1 = *tab++;
                p->t.box2 = *tab++;
                p->t.box3 = *tab++;
                p->t.d84 = 0x1000;
                p->t.d88 = 0x1000;
                p->t.d8c = 0x1000;
                if (p->t.d94 == 0 && (o->t.subtype < 2 || o->t.subtype == 3) && (q = FUN_800183b8()) != 0) {
                    q[0] = 1;
                    b = o->t.b1d;
                    q[2] = 2;
                    q[0x1d] = b;
                    if (o->t.subtype == 3) q[3] = 7; else q[3] = 2;
                    *(short *)(q + 0x2e) = 1;
                    q[0xc] = 0;
                    *(int *)(q + 0x10) = D_80138814[o->t.subtype].x << 16;
                    *(int *)(q + 0x14) = D_80138814[o->t.subtype].y << 16;
                    fx = *o->t.d;
                    *(TO2 **)(q + 0x94) = p;
                    *(Fix16 *)(q + 0x18) = fx;
                }
                p = (TO2 *)p->t.d94;
                if (p == 0) break;
                p->dc4 = p->t.a.raw - x;
                p->dc8 = p->t.y.raw - y;
                p->dcc = p->t.b.raw - z;
                x = p->t.a.raw;
                y = p->t.y.raw;
                z = p->t.b.raw;
            }
        }
        o->t.b04++;
        break;
    case 1:
        if (o->t.d90 == 0) {
            if (*(unsigned short *)&o->t.wbc != 0) {
                if (--*(unsigned short *)&o->t.wbc == 0) {
                    o->t.wbc = D_80138824[(*(unsigned short *)&o->t.bbe)++];
                    FUN_8001e4f0(0x2d);
                }
            }
            f = 0;
            if (o->t.b69 == 1) {
                f = 1;
                o->t.wb4 = 1;
                o->t.wbc = 1;
                *(unsigned short *)&o->t.bbe = 0;
            }
            if (o->t.b68 == 1) {
                f = 1;
                w = D_801387E4[o->t.subtype][o->t.b6b];
                o->t.wbc = 1;
                *(unsigned short *)&o->t.bbe = 0;
                o->t.wb4 = w;
            }
            p = o;
            n = 0;
            x = o->t.a.raw;
            y = o->t.y.raw;
            z = o->t.b.raw;
            o->t.b69 = 0;
            o->t.b68 = 0;
            m.t[0] = 0;
            m.t[1] = 0;
            m.t[2] = 0;
            for (;;) {
                if (f) {
                    t1 = o->t.wb4;
                    p->t.wb4 = t1;
                    p->t.wb8 = D_80138718[t1][n + D_80138720[o->t.subtype]];
                }
                r.vx = 0;
                r.vy = 0;
                if (o->t.animFrame == 0) {
                    r.vz = 0x1000 - D_8007A3F0[D_801386B4[(unsigned short)p->t.wb8]] / D_80138748[(unsigned short)p->t.wb4][n + D_80138720[o->t.subtype]];
                } else {
                    r.vz = D_8007A3F0[D_801386B4[(unsigned short)p->t.wb8]] / D_80138748[(unsigned short)p->t.wb4][n + D_80138720[o->t.subtype]];
                }
                FUN_8006424c(&r, &m);
                if (p->t.d94 == 0) {
                    p->t.d84 = r.vx;
                    p->t.d88 = r.vy;
                    p->t.d8c = r.vz * 2 - 0x1000;
                }
                v.vx = p->dc4;
                v.vy = p->dc8;
                v.vz = p->dcc;
                FUN_80063a6c(&m, &v, &out);
                x += out.vx;
                y += out.vy;
                z += out.vz;
                p->t.a.raw = x;
                p->t.b.raw = z;
                if (p->t.d94 == 0) {
                    c = D_801386B4[(unsigned short)p->t.wb8];
                    if (c < 0)
                        p->t.y.raw = y + ((signed char)(c / 10) << 16);
                    else
                        p->t.y.raw = y - ((signed char)(c / 10) << 16);
                } else {
                    p->t.y.raw = y;
                }
                nx = (TO2 *)p->t.d94;
                if (nx == 0) break;
                p = nx;
                t = p->t.wb8;
                p->t.wb8 = t + 1;
                if (D_801386B4[(unsigned short)(t + 1)] == 0x7f) {
                    p->t.wb8 = t;
                    p->t.wb4 = 0;
                }
                n++;
            }
        }
        ObjCullRegister(&o->t);
        break;
    case 2:
        o->t.b04++;
        break;
    case 3:
        ObjFreeDup(&o->t);
        break;
    }
}
