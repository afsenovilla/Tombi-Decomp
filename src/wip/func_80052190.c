// FUNC 80052190 2012 MAIN0
// FLAGS -O2 -G0 -fno-strength-reduce
/* score 447: rotated-sprite twin of func_8005296C. Logic/branch layout match; diff is
 * loop-invariant hoisting + spills. Game hoists &A[a],&B[a],&A[b],&B[b], d*2 (+&B[d] only),
 * &A[c],&B[c] (c=(b+0xc0)&0xff, d=(a+0x40)&0xff) but NOT the (a+0xc0)/(b+0x40) chains; a,b live
 * spilled at sp32/sp40. gcc loop.c drops threshold by 3 per moved reg (T0=29, ~360 insns), so with
 * c,d as vars c<<1 is the 11th move and is rejected. Tried: explicit ptr vars for a/b/c/d (36 combos),
 * local table pointers, type brute force of a..d, branch swap. `short x=(signed char)f[2]` is the
 * right form for the lbu+sll/sra24 then sll/sra16 pattern. */
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

void func_80052190(TObj *o)
{
    PolyFT4 *p;
    short *e;
    char *q;
    unsigned char *f;
    int n;
    unsigned long xy;
    int cx, cy;
    int a, b, c, d;
    short *ac, *as, *bc, *bs, *cc, *cs, *dc, *ds;
    short dx, dy;
    int ndx, ndy, dx2, dy2;
    unsigned char w, h;
    int v;

    D_1F800060[0] = o->a.p.whole;
    D_1F800062 = o->y.p.whole;
    if (D_8009C960 == 0x10005 && D_1F8001D4->w4c != 3)
        D_1F800064 = o->b.p.whole >> 2;
    else
        D_1F800064 = o->b.p.whole;
    SetRotMatrix(D_1F8000C0);
    SetTransMatrix(D_1F8000C0b);
    if (proj()) return;
    xy = D_1F800070;
    cx = xy;
    cy = xy >> 16;
    a = (o->d8c + 0x80) & 0xff;
    b = o->d8c & 0xff;
    c = (b + 0xc0) & 0xff;
    cc = &D_8007A5F0[c];
    cs = &D_8007A3F0[c];
    e = (short *)(o->d3c + *(unsigned short *)o->anim * 4);
    n = e[0];
    q = (char *)(*(volatile int *)&o->d3c + e[1]);
    f = (unsigned char *)q + 0xc;
    do {
        short x = (signed char)f[2];
        short y = (signed char)f[3];
        p = DAT_1f800164;
        w = f[-2];
        h = f[-1];
        if (!(o->animFrame & 1)) {
            dx = x;
            dy = y;
            ndx = -dx;
            ndy = -dy;
            p->x0 = cx + ((ndx * D_8007A5F0[a]) >> 12) + ((ndy * D_8007A5F0[(a + 0xc0) & 0xff]) >> 12);
            p->y0 = cy + ((ndx * D_8007A3F0[a]) >> 12) + ((ndy * D_8007A3F0[(a + 0xc0) & 0xff]) >> 12);
            dx2 = w + dx;
            p->x1 = cx + ((dx2 * D_8007A5F0[b]) >> 12) + ((ndy * D_8007A5F0[(b + 0x40) & 0xff]) >> 12);
            p->y1 = cy + ((dx2 * D_8007A3F0[b]) >> 12) + ((ndy * D_8007A3F0[(b + 0x40) & 0xff]) >> 12);
            dy2 = h + dy;
            p->x2 = cx + ((ndx * D_8007A5F0[a]) >> 12) + ((dy2 * D_8007A5F0[(a + 0x40) & 0xff]) >> 12);
            p->y2 = cy + ((ndx * D_8007A3F0[a]) >> 12) + ((dy2 * D_8007A3F0[(a + 0x40) & 0xff]) >> 12);
            p->x3 = cx + ((dx2 * D_8007A5F0[b]) >> 12) + ((dy2 * *cc) >> 12);
            p->y3 = cy + ((dx2 * D_8007A3F0[b]) >> 12) + ((dy2 * *cs) >> 12);
        } else {
            dx = -(x + w);
            dy = y;
            ndx = -dx;
            ndy = -dy;
            p->x1 = cx + ((ndx * D_8007A5F0[a]) >> 12) + ((ndy * D_8007A5F0[(a + 0xc0) & 0xff]) >> 12);
            p->y1 = cy + ((ndx * D_8007A3F0[a]) >> 12) + ((ndy * D_8007A3F0[(a + 0xc0) & 0xff]) >> 12);
            dx2 = w + dx;
            p->x0 = cx + ((dx2 * D_8007A5F0[b]) >> 12) + ((ndy * D_8007A5F0[(b + 0x40) & 0xff]) >> 12);
            p->y0 = cy + ((dx2 * D_8007A3F0[b]) >> 12) + ((ndy * D_8007A3F0[(b + 0x40) & 0xff]) >> 12);
            dy2 = h + dy;
            p->x3 = cx + ((ndx * D_8007A5F0[a]) >> 12) + ((dy2 * D_8007A5F0[(a + 0x40) & 0xff]) >> 12);
            p->y3 = cy + ((ndx * D_8007A3F0[a]) >> 12) + ((dy2 * D_8007A3F0[(a + 0x40) & 0xff]) >> 12);
            p->x2 = cx + ((dx2 * D_8007A5F0[b]) >> 12) + ((dy2 * *cc) >> 12);
            p->y2 = cy + ((dx2 * D_8007A3F0[b]) >> 12) + ((dy2 * *cs) >> 12);
        }
        p->code = 0x2c;
        if (o->subtype == 0)
            SetSemiTrans(p, 1);
        else
            SetSemiTrans(p, 0);
        p->tpage = o->w1e;
        p->clut = o->w08;
        v = 0x80;
        if (o->subtype == 0) {
            if (o->b0d & 0x40)
                v = 0x20;
            else
                v = 0x40;
        }
        p->r0 = v;
        p->g0 = v;
        p->b0 = v;
        p->uv0 = *(unsigned short *)q + 0x80;
        p->uv1 = *(unsigned short *)(f - 8) + 0x80;
        p->uv2 = *(unsigned short *)(f - 4) + 0x80;
        p->uv3 = *(unsigned short *)f + 0x80;
        if (!otadd((unsigned long *)p, DAT_1f8001e0 + 0x10, D_1F800074, o->b0f, 0x9000000))
            DAT_1f800164 = (PolyFT4 *)((char *)DAT_1f800164 + 0x28);
        q += 0x10;
        f += 0x10;
        n--;
    } while (n != 0);
}
