// FUNC 800505f0 2228 MAIN0
// FLAGS -O2 -G0 -fno-strength-reduce
/* score 517: twin of wip func_80052190 (rotated FT4 sprite). Logic decoded from asm; game frame 0x80 spills t1-t9 around SetSemiTrans (caller-save) and keeps q at sp+0x18, count at sp+0x10; ours frame 0x60. Register allocation not attempted further. */
#include "TOBJ.H"
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    unsigned short x0, y0;
    unsigned short uv0, clut;
    unsigned short x1, y1;
    unsigned short uv1, tpage;
    unsigned short x2, y2;
    unsigned short uv2, pad2;
    unsigned short x3, y3;
    unsigned short uv3, pad3;
} PolyFT4;
typedef struct { char pad[0x4c]; unsigned short w4c; } C4C;
extern PolyFT4 *DAT_1f800164;
extern char *DAT_1f8001e0;
extern int D_8009C960;
extern C4C *D_1F8001D4;
extern short D_1F800060[];
extern short D_1F800062, D_1F800064;
extern long D_1F80008C;
extern unsigned long D_1F800070;
extern long D_1F800074;
extern char D_1F8000C0[], D_1F8000C0b[];
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern void SetSemiTrans(void *, int);
extern short D_8007A5F0[];
extern short D_8007A3F0[];
typedef struct { unsigned char p0[7]; signed char b7; unsigned char p8[0x18 - 8]; short w18, w1a; unsigned char p1c[0x28 - 0x1c]; unsigned short w28[1]; } P800505F0;
extern P800505F0 *D_8009C330;
extern short D_1F8001F4;
extern unsigned short D_8009C960h, D_8009C962h;
extern void FUN_80037c80(TObj *);

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

static __inline__ int onscreen(PolyFT4 *p)
{
    if (p->y0 < 0x100 || p->y1 < 0x100 || p->y2 < 0x100 || p->y3 < 0x100) {
        if (p->x0 < 0x140 || p->x1 < 0x140 || p->x2 < 0x140 || p->x3 < 0x140)
            return 1;
    }
    return 0;
}

static __inline__ int otadd(unsigned long *a, char *b, int c, int d, unsigned long e)
{
    d = ((signed char)d + c) << 2;
    if (d < 0) d = 0;
    d += (int)b;
    if ((unsigned)(d - (int)DAT_1f8001e0) >= 0xca0) return 1;
    {
        unsigned long v = *(unsigned long *)d;
        *(unsigned long *)d = (unsigned long)a;
        *a = v | e;
    }
    return 0;
}

void FUN_800505f0(TObj *o)
{
    PolyFT4 *p;
    short *e;
    unsigned char *q;
    unsigned char *f;
    short n;
    short s;
    unsigned long xy;
    int cx, cy;
    int a, b;
    short dx, dy;
    int ndx, ndy;
    int v;
    unsigned short af;
    int x, y;

    D_1F800060[0] = o->a.p.whole;
    D_1F800062 = o->y.p.whole;
    if (D_8009C960 == 0x10005 && D_1F8001D4->w4c != 3)
        D_1F800064 = o->b.p.whole >> 2;
    else
        D_1F800064 = o->b.p.whole;
    SetRotMatrix(D_1F8000C0);
    SetTransMatrix(D_1F8000C0b);
    if (proj()) return;
    s = *(short *)o->anim;
    xy = D_1F800070;
    e = (short *)(o->d3c + s * 4);
    q = (unsigned char *)(o->d3c + e[1]);
    n = e[0];
    cy = xy >> 16;
    cx = xy;
    if (D_8009C330->w28[D_1F8001F4] != s) {
        if (D_8009C960h == 6 && (D_8009C962h >= 2 || (short)D_8009C962h < 0))
            FUN_80037c80(o);
        FUN_80037c80(o);
    }
    a = (o->d8c + 0x80) & 0xff;
    b = o->d8c & 0xff;
    f = q + 0xb;
    D_8009C330->w18 = cx;
    D_8009C330->w1a = cy;
    do {
        x = (signed char)f[3];
        y = (signed char)f[4];
        p = DAT_1f800164;
        p->code = 0x2d;
        SetSemiTrans(p, 0);
        p->tpage = o->w1e;
        if (n & 2)
            p->clut = o->w08;
        else
            p->clut = *(unsigned short *)((char *)o + 0xc4);
        ((unsigned char *)&p->uv0)[0] = q[0] + ((unsigned char)D_1F8001F4 << 6) + 4;
        ((unsigned char *)&p->uv0)[1] = f[-10] + 4;
        ((unsigned char *)&p->uv2)[0] = ((unsigned char *)&p->uv0)[0];
        ((unsigned char *)&p->uv1)[1] = ((unsigned char *)&p->uv0)[1];
        ((unsigned char *)&p->uv1)[0] = ((unsigned char *)&p->uv0)[0] + f[-1];
        ((unsigned char *)&p->uv2)[1] = ((unsigned char *)&p->uv0)[1] + f[0];
        ((unsigned char *)&p->uv3)[0] = ((unsigned char *)&p->uv0)[0] + f[-1];
        ((unsigned char *)&p->uv3)[1] = ((unsigned char *)&p->uv0)[1] + f[0];
        if (o->b9d)
            af = D_8009C330->b7;
        else
            af = o->animFrame;
        if (af & 1) {
            dx = -(x + f[-1]);
            dy = y;
            ndx = -dx;
            ndy = -dy;
            p->x1 = cx + ((ndx * D_8007A5F0[a]) >> 12) + ((ndy * D_8007A5F0[(a + 0xc0) & 0xff]) >> 12);
            p->y1 = cy + ((ndx * D_8007A3F0[a]) >> 12) + ((ndy * D_8007A3F0[(a + 0xc0) & 0xff]) >> 12);
            p->x0 = cx + (((dx + f[-1]) * D_8007A5F0[b]) >> 12) + ((ndy * D_8007A5F0[(b + 0x40) & 0xff]) >> 12);
            p->y0 = cy + (((dx + f[-1]) * D_8007A3F0[b]) >> 12) + ((ndy * D_8007A3F0[(b + 0x40) & 0xff]) >> 12);
            p->x3 = cx + ((ndx * D_8007A5F0[a]) >> 12) + (((dy + f[0]) * D_8007A5F0[(a + 0x40) & 0xff]) >> 12);
            p->y3 = cy + ((ndx * D_8007A3F0[a]) >> 12) + (((dy + f[0]) * D_8007A3F0[(a + 0x40) & 0xff]) >> 12);
            p->x2 = cx + (((dx + f[-1]) * D_8007A5F0[b]) >> 12) + (((dy + f[0]) * D_8007A5F0[(b + 0xc0) & 0xff]) >> 12);
            p->y2 = cy + (((dx + f[-1]) * D_8007A3F0[b]) >> 12) + (((dy + f[0]) * D_8007A3F0[(b + 0xc0) & 0xff]) >> 12);
        } else {
            dx = x;
            dy = y;
            ndx = -dx;
            ndy = -dy;
            p->x0 = cx + ((ndx * D_8007A5F0[a]) >> 12) + ((ndy * D_8007A5F0[(a + 0xc0) & 0xff]) >> 12);
            p->y0 = cy + ((ndx * D_8007A3F0[a]) >> 12) + ((ndy * D_8007A3F0[(a + 0xc0) & 0xff]) >> 12);
            p->x1 = cx + (((dx + f[-1]) * D_8007A5F0[b]) >> 12) + ((ndy * D_8007A5F0[(b + 0x40) & 0xff]) >> 12);
            p->y1 = cy + (((dx + f[-1]) * D_8007A3F0[b]) >> 12) + ((ndy * D_8007A3F0[(b + 0x40) & 0xff]) >> 12);
            p->x2 = cx + ((ndx * D_8007A5F0[a]) >> 12) + (((dy + f[0]) * D_8007A5F0[(a + 0x40) & 0xff]) >> 12);
            p->y2 = cy + ((ndx * D_8007A3F0[a]) >> 12) + (((dy + f[0]) * D_8007A3F0[(a + 0x40) & 0xff]) >> 12);
            p->x3 = cx + (((dx + f[-1]) * D_8007A5F0[b]) >> 12) + (((dy + f[0]) * D_8007A5F0[(b + 0xc0) & 0xff]) >> 12);
            p->y3 = cy + (((dx + f[-1]) * D_8007A3F0[b]) >> 12) + (((dy + f[0]) * D_8007A3F0[(b + 0xc0) & 0xff]) >> 12);
        }
        if (!otadd((unsigned long *)p, DAT_1f8001e0 + 0x10, D_1F800074, o->b0f, 0x9000000))
            DAT_1f800164 = (PolyFT4 *)((char *)DAT_1f800164 + 0x28);
        q += 0x10;
        f += 0x10;
    } while (--n);
}
