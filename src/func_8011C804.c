// FUNC 8011c804 2060 X000
// MATCHING 8011c804 2060
/* Real size 2060 B: splat split it into func_8011C804 (508), func_8011CA00 (728) and func_8011CCD8 (824). */
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { long vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { unsigned short frac; short whole; } FixParts;
typedef union { int raw; FixParts p; } Fix16;
typedef struct { short x, y, z; } XYZ;

typedef struct E {
    unsigned char active;
    unsigned char visible;
    unsigned char type;
    unsigned char subtype;
    unsigned char b04;
    char p05[0xc - 5];
    unsigned char b0c;
    char p0d[0x10 - 0xd];
    Fix16 a, y, b;
    char p1c;
    unsigned char b1d;
    char p1e[0x2e - 0x1e];
    unsigned short animFrame;
    int d30, d34, d38;
    char p3c[0x68 - 0x3c];
    unsigned char b68, b69, b6a, b6b;
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
    short *p[7];
} E;

extern short D_801389B8[], D_801389BC[], D_801389DC[];
extern short D_80138A10[], D_80138A64[], D_80138A30[], D_80138A84[];
extern short D_80138848[], D_8013884C[], D_8013886C[];
extern short D_801388C8[][4], D_80138908[][4], D_80138948[][4];
extern XYZ D_80138988[];
extern short D_800A6052;
extern E *FUN_800183b8(void);
extern void FUN_800202b4(E *);
extern void FUN_800201ac(E *, int);
extern void FUN_8006424c(SVECTOR *, MATRIX *);
extern void FUN_80063a6c(MATRIX *, VECTOR *, VECTOR *);
extern void FUN_80018838(E *);

#define ADV(q) { short *t = q; q = t + 1; if ((unsigned short)t[1] == 0x8000) q = t; }

void func_8011C804(E *o)
{
    E *e;
    E *pr;
    E *n;
    int x, y, z;
    int flag;
    short *t0, *t1, *t2;
    short **pp;
    VECTOR in;
    VECTOR out;
    SVECTOR v;
    MATRIX m;

    switch (o->b04) {
    case 0:
        o->b6b = 0;
        o->ba4 = 0;
        if (o->d90 == 0) {
            e = o;
            pr = o;
            x = o->a.raw;
            y = o->y.raw;
            z = o->b.raw;
            for (;;) {
                e->wb4 = 0;
                e->wb6 = 0;
                e->p[0] = D_801389B8;
                e->p[1] = D_801389B8;
                e->p[2] = D_801389B8;
                e->p[3] = D_801388C8[o->subtype];
                e->p[4] = D_80138908[o->subtype];
                e->p[5] = D_80138948[o->subtype];
                if (e->next == 0) {
                    e->box0 = 20;
                    e->box1 = 40;
                    e->box2 = 15;
                    e->box3 = 30;
                    e->p[6] = D_80138848;
                    if (o->subtype == 4 && (n = FUN_800183b8()) != 0) {
                        n->active = 1;
                        n->b1d = o->b1d;
                        n->type = 2;
                        n->animFrame = 1;
                        n->a.raw = 0x7980000;
                        n->subtype = 4;
                        n->b0c = 0;
                        n->y.raw = 0xfece0000;
                        n->b.raw = 0;
                        n->next = e;
                    }
                } else {
                    e->active = 2;
                    e->p[6] = D_80138848;
                }
                e = e->next;
                if (e == 0) break;
                pr->d30 = e->a.raw - x;
                pr->d34 = e->y.raw - y;
                pr->d38 = e->b.raw - z;
                x = e->a.raw;
                y = e->y.raw;
                z = e->b.raw;
                pr = e;
            }
        }
        o->b04++;
        break;
    case 1:
        if (D_800A6052 >= 46 && o->subtype == 0) break;
        if (o->b0c == 0) FUN_800202b4(o);
        else FUN_800201ac(o, 64);
        if (o->visible == 0) break;
        if (o->d90 != 0) break;
        for (e = o; ; e = e->next) {
            if (e->next == 0) {
                flag = 0;
                switch (e->b69) {
                case 1:
                case 3:
                case 4:
                    if (o->wb6 == 0) {
                        flag = 1;
                        o->wb4 = e->b69;
                        o->wb6 = 4;
                    }
                    break;
                default:
                    if (o->wb6 != 0) o->wb6--;
                    break;
                }
                switch (e->b68) {
                case 1:
                    o->wb4 = 2;
                    flag = 1;
                    break;
                case 2:
                    if (o->wb6 == 0) {
                        flag = 1;
                        o->wb4 = 4;
                        o->wb6 = 4;
                    }
                    break;
                }
                e->b69 = 0;
                e->b68 = 0;
                o->animFrame = e->animFrame;
                break;
            }
        }
        e = o;
        x = o->a.raw;
        y = o->y.raw;
        z = o->b.raw;
        t0 = o->p[3];
        t1 = o->p[4];
        t2 = o->p[5];
        for (;;) {
            pp = e->p;
            if (flag) {
                e->wb4 = o->wb4;
                switch (o->wb4) {
                case 1:
                    pp[0] = D_801389BC;
                    pp[6] = D_8013884C;
                    break;
                case 2:
                    pp[1] = D_80138A10;
                    pp[2] = D_80138A64;
                    pp[6] = D_8013884C;
                    break;
                case 3:
                    pp[0] = D_801389DC;
                    pp[6] = D_8013886C;
                    break;
                case 4:
                    pp[1] = D_80138A30;
                    pp[2] = D_80138A84;
                    pp[6] = D_8013886C;
                    break;
                }
            }
            v.vx = *pp[0] / *t0;
            if (o->animFrame == 0) {
                v.vy = 0x1000 - *pp[1] / *t1;
                v.vz = 0x1000 - *pp[2] / *t2;
            } else {
                v.vy = *pp[1] / *t1;
                v.vz = *pp[2] / *t2;
            }
            if (e->subtype == 0) v.vz += 0xed4;
            FUN_8006424c(&v, &m);
            if (e->next == 0) {
                e->d84 = D_80138988[e->subtype].x + v.vx;
                e->d88 = D_80138988[e->subtype].y + v.vy;
                e->d8c = D_80138988[e->subtype].z + v.vz + *pp[6];
                if (e->subtype == 0) e->d8c += 300;
            } else {
                e->d84 = v.vx;
                e->d88 = v.vy;
                e->d8c = v.vz;
            }
            ADV(pp[0]);
            ADV(pp[1]);
            ADV(pp[2]);
            ADV(pp[6]);
            t1++;
            t0++;
            t2++;
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
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
