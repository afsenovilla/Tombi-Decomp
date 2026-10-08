// FUNC 80051088 4360 MAIN0
/* score 938 (first draft, structure complete; frame matches 120 once otadd is an inline taking
   DAT_1f8001e0 + 0x10, a macro version adds 8 B of stack per use).
   Left: register allocation (game: s0=idx, s1/s2 temps, s3=p, s4=o, s5=x, s6=y, s7=a0, fp=a1),
   tbl[] reads must be lbu + sll/sra (ours lb) so t1 gets an explicit short conversion in each branch,
   both D_800A60E8 branches kept separate with their own calls, rotating-quad block scheduling. */
#include "TOBJ.H"

typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} FT4;

typedef struct {
    short x, y;
    unsigned short w4, w6, w8, w10;
    signed char b12;
    unsigned char b13;
    short w14;
    unsigned short q1[8];
    unsigned short q2[8];
    short w48, w50;
} SH;

typedef struct { short x, y; } P;
typedef struct { char pad[7]; signed char b7; char pad8[0x18 - 8]; unsigned short w18, w1a; } PL;
typedef struct { char pad[0x4c]; unsigned short w4c; } C4C;

extern FT4 *DAT_1f800164;
extern char *DAT_1f8001e0;
extern int D_8009C960;
extern C4C *D_1F8001D4;
extern short D_1F800060[];
extern short D_1F800062, D_1F800064;
extern long D_1F80008C;
extern long D_1F800070;
extern long D_1F800074;
extern unsigned short D_1F8001F8;
extern char D_1F8000C0[], D_1F8000C0b[];
extern SH *D_8009C338;
extern PL *D_8009C330;
extern short D_800A6066;
extern unsigned char D_800A60D5;
extern unsigned short *D_800A605C;
extern unsigned char D_800A6100;
extern int D_800A60C4;
extern short D_800A60E8;
extern short D_800801E8, D_800801EA, D_800801EC, D_800801EE;
extern short costab[];
extern short negsintab2[];
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern void SetSemiTrans(void *, int);
extern void FUN_8005a014(TObj *, int, int);
extern int MulCos(int, short);
extern int MulCosDup(int, short);
extern int MulNegSin(int, short);
extern int MulNegSinScaled(int, short);
extern int FUN_8002078c(P, P);

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

static __inline__ int proj(void)
{
    gte_ldv0(D_1F800060);
    gte_rtps();
    gte_stflg(&D_1F80008C);
    if (D_1F80008C < 0) return 1;
    gte_stsxy(&D_1F800070);
    gte_stszotz(&D_1F800074);
    return 0;
}

static __inline__ int otadd(unsigned long *a, char *b, int c, short d, unsigned long e)
{
    int k = c * 4;
    k += d * 4;
    if (k < 0) k = 0;
    k += (int)b;
    if ((unsigned)(k - (int)DAT_1f8001e0) >= 0xca0) return 1;
    {
        unsigned long v = *(unsigned long *)k;
        *(unsigned long *)k = (unsigned long)a;
        *a = v | e;
    }
    return 0;
}

#define COS(i) costab[i]
#define SIN(i) negsintab2[i]

#define SHADOW(q) { \
    p = DAT_1f800164; \
    FUN_8005a014(o, D_8009C338->x, D_8009C338->y); \
    p->code = 0x2c; \
    SetSemiTrans(p, 1); \
    p->tpage = D_8009C338->w8; \
    p->clut = D_8009C338->w10; \
    p->u0 = 0xc0; \
    p->u2 = 0xc0; \
    p->v0 = 0; \
    p->u1 = 0xcf; \
    p->v1 = 0; \
    p->v2 = 0xf; \
    p->u3 = 0xcf; \
    p->v3 = 0xf; \
    p->x0 = q[0]; \
    p->x1 = q[2]; \
    p->x2 = q[4]; \
    p->x3 = q[6]; \
    p->y0 = q[1]; \
    p->y1 = q[3]; \
    p->y2 = q[5]; \
    p->y3 = q[7]; \
    p->r0 = 0x40; \
    p->g0 = 0x40; \
    p->b0 = 0x40; \
    if (otadd((unsigned long *)p, DAT_1f8001e0 + 0x10, D_1F800074, (signed char)o->b0f - 5, 0x9000000) == 0) \
        DAT_1f800164 = DAT_1f800164 + 1; \
}

void func_80051088(TObj *o)
{
    P s;
    P t;
    unsigned char tbl[36] = {
        0x00, 0x00, 0x06, 0x16, 0x54, 0x18, 0x00, 0x00, 0x1f, 0x18, 0x60, 0x18,
        0x00, 0x00, 0x40, 0x1d, 0x6f, 0x17, 0x00, 0x00, 0x08, 0x16, 0x58, 0x18,
        0x00, 0x00, 0x24, 0x1a, 0x68, 0x1a, 0x00, 0x00, 0x3c, 0x1a, 0x70, 0x16 };
    short idx;
    short x, y;
    unsigned int a0, a1;
    FT4 *p;
    int ang, c, sn, r;
    short k;
    short t1;

    D_1F800060[0] = o->a.p.whole;
    D_1F800062 = o->y.p.whole;
    if (D_8009C960 == 0x10005 && D_1F8001D4->w4c != 3)
        D_1F800064 = o->b.p.whole >> 2;
    else
        D_1F800064 = o->b.p.whole;
    SetRotMatrix(D_1F8000C0);
    SetTransMatrix(D_1F8000C0b);
    if (proj()) return;
    idx = 0;
    x = D_1F800070;
    y = (unsigned long)D_1F800070 >> 16;
    D_8009C338->x = x;
    D_8009C338->y = y;
    a0 = o->d8c & 0xff;
    a1 = (o->d8c + 0x80) & 0xff;
    if (D_800A60D5)
        k = D_8009C330->b7;
    else
        k = D_800A6066;
    switch (k) {
    case 0: case 1: case 2: case 3:
        switch (*D_800A605C) {
        case 0x8b: break;
        case 0x94: idx += 2; break;
        case 0x97: idx += 4; break;
        case 0x9a: case 0x9b: case 0x9c: idx += 0x12; break;
        case 0xa3: idx += 0x14; break;
        case 0xa6: idx += 0x16; break;
        }
        break;
    case 4: case 5:
        switch (*D_800A605C) {
        case 0x8e: case 0x8f: case 0x90: idx += 6; break;
        case 0x95: idx += 8; break;
        case 0x98: idx += 10; break;
        case 0x9d: case 0x9e: case 0x9f: idx += 0x18; break;
        case 0xa4: idx += 0x1a; break;
        case 0xa7: idx += 0x1c; break;
        }
        break;
    case 6: case 7:
        switch (*D_800A605C) {
        case 0x91: case 0x92: case 0x93: idx += 0xc; break;
        case 0x96: idx += 0xe; break;
        case 0x99: idx += 0x10; break;
        case 0xa0: case 0xa1: case 0xa2: idx += 0x1e; break;
        case 0xa5: idx += 0x20; break;
        case 0xa8: idx += 0x22; break;
        }
        break;
    }
    if (D_8009C338->b12) {
        if (D_8009C338->w48 > 0)
            SHADOW(D_8009C338->q1);
        if (D_8009C338->w50 > 0)
            SHADOW(D_8009C338->q2);
        p = DAT_1f800164;
        p->code = 0x2d;
        SetSemiTrans(p, 0);
        p->tpage = D_8009C338->w4;
        p->clut = D_8009C338->w10;
        p->u0 = D_8009C338->b13 * 32 - 0x80;
        p->v0 = 0x10;
        p->u1 = D_8009C338->b13 * 32 - 0x61;
        p->v1 = 0x10;
        p->u2 = D_8009C338->b13 * 32 - 0x80;
        p->v2 = 0x1f;
        p->u3 = D_8009C338->b13 * 32 - 0x61;
        p->v3 = 0x1f;
        s.x = D_8009C338->x;
        s.y = D_8009C338->y;
        if (D_800A6100 && *D_800A605C >= 0xa9) {
            c = MulCosDup((D_800A60C4 + 0x40) & 0xff, 0x18);
            sn = MulNegSin((D_800A60C4 + 0x40) & 0xff, 0x18);
        } else {
            int t0 = (signed char)tbl[idx];
            int t1i = (signed char)tbl[idx + 1];
            if (o->animFrame & 1) {
                if (D_800A60E8 > 0) {
                    ang = (D_800A60C4 + 0x180 - t0) & 0xff;
                    c = MulCosDup(ang, t1i);
                    sn = MulNegSin(ang, t1i);
                } else {
                    ang = (D_800A60C4 + 0x180 - t0) & 0xff;
                    c = MulCosDup(ang, t1i);
                    sn = MulNegSin(ang, t1i);
                }
            } else {
                ang = (t0 + D_800A60C4) & 0xff;
                c = MulCosDup(ang, t1i);
                sn = MulNegSin(ang, t1i);
            }
        }
        t.x = D_8009C330->w18 + c;
        t.y = D_8009C330->w1a + sn;
        r = FUN_8002078c(t, s);
        {
            int b1 = (r + 0x140) & 0xff;
            int b2;
            p->x0 = t.x + MulCos(b1, 8);
            p->y0 = t.y + MulNegSinScaled(b1, 8);
            b2 = (r + 0xc0) & 0xff;
            p->x2 = t.x + MulCos(b2, 8);
            p->y2 = t.y + MulNegSinScaled(b2, 8);
            p->x1 = s.x + MulCos(b1, 8);
            p->y1 = s.y + MulNegSinScaled(b1, 8);
            p->x3 = s.x + MulCos(b2, 8);
            p->y3 = s.y + MulNegSinScaled(b2, 8);
        }
        if (otadd((unsigned long *)p, DAT_1f8001e0 + 0x10, D_1F800074, (signed char)o->b0f - 3, 0x9000000) == 0)
            DAT_1f800164 = DAT_1f800164 + 1;
    }
    p = DAT_1f800164;
    p->code = 0x2d;
    SetSemiTrans(p, o->b0d >> 7);
    p->tpage = D_8009C338->w4;
    p->v0 = 0;
    p->u0 = (D_1F8001F8 & 3) * D_800801E8 - 0x80;
    p->u1 = p->u0 + (unsigned char)D_800801E8 - 1;
    p->v1 = p->v0;
    p->u2 = p->u0;
    p->v2 = p->v0 + (unsigned char)D_800801EA - 1;
    p->clut = D_8009C338->w6;
    p->u3 = p->u1;
    p->v3 = p->v2;
    if (D_800A6100 && *D_800A605C >= 0xa9) {
        if (o->animFrame & 1)
            a0 = D_800A60C4 + 0xc0;
        else
            a0 = D_800A60C4 + 0x40;
        a0 &= 0xff;
        a1 = (a0 + 0x80) & 0xff;
    }
    if (o->animFrame & 1) {
        int m, n2, m2, n;
        D_800801EC = -(D_800801EC + D_800801E8);
        m = -D_800801EC;
        n = -D_800801EE;
        p->x1 = x + ((m * COS(a1)) >> 12) + ((n * COS((a1 + 0xc0) & 0xff)) >> 12);
        p->y1 = y + ((m * SIN(a1)) >> 12) + ((n * SIN((a1 + 0xc0) & 0xff)) >> 12);
        m2 = D_800801EC + D_800801E8;
        p->x0 = x + ((m2 * COS(a0)) >> 12) + ((n * COS((a0 + 0x40) & 0xff)) >> 12);
        p->y0 = y + ((m2 * SIN(a0)) >> 12) + ((n * SIN((a0 + 0x40) & 0xff)) >> 12);
        n2 = D_800801EE + D_800801EA;
        p->x3 = x + ((m * COS(a1)) >> 12) + ((n2 * COS((a1 + 0x40) & 0xff)) >> 12);
        p->y3 = y + ((m * SIN(a1)) >> 12) + ((n2 * SIN((a1 + 0x40) & 0xff)) >> 12);
        p->x2 = x + ((m2 * COS(a0)) >> 12) + ((n2 * COS((a0 + 0xc0) & 0xff)) >> 12);
        p->y2 = y + ((m2 * SIN(a0)) >> 12) + ((n2 * SIN((a0 + 0xc0) & 0xff)) >> 12);
    } else {
        int m, n2, m2, n;
        m = -D_800801EC;
        n = -D_800801EE;
        p->x0 = x + ((m * COS(a1)) >> 12) + ((n * COS((a1 + 0xc0) & 0xff)) >> 12);
        p->y0 = y + ((m * SIN(a1)) >> 12) + ((n * SIN((a1 + 0xc0) & 0xff)) >> 12);
        m2 = D_800801EC + D_800801E8;
        p->x1 = x + ((m2 * COS(a0)) >> 12) + ((n * COS((a0 + 0x40) & 0xff)) >> 12);
        p->y1 = y + ((m2 * SIN(a0)) >> 12) + ((n * SIN((a0 + 0x40) & 0xff)) >> 12);
        n2 = D_800801EE + D_800801EA;
        p->x2 = x + ((m * COS(a1)) >> 12) + ((n2 * COS((a1 + 0x40) & 0xff)) >> 12);
        p->y2 = y + ((m * SIN(a1)) >> 12) + ((n2 * SIN((a1 + 0x40) & 0xff)) >> 12);
        p->x3 = x + ((m2 * COS(a0)) >> 12) + ((n2 * COS((a0 + 0xc0) & 0xff)) >> 12);
        p->y3 = y + ((m2 * SIN(a0)) >> 12) + ((n2 * SIN((a0 + 0xc0) & 0xff)) >> 12);
    }
    if (otadd((unsigned long *)p, DAT_1f8001e0 + 0x10, D_1F800074, (signed char)o->b0f - 4, 0x9000000) == 0)
        DAT_1f800164 = DAT_1f800164 + 1;
}
