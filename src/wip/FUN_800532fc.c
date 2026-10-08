// FUNC 800532fc 2144 MAIN0
/* score 338 (ncheck): only register allocation/loop-invariant hoisting differs. Game hoists &A[a],&C[a],&A[b],&C[b], idx((a+0x40)&0xff)*2 (t9), and spills &C[(a+0x40)], &A[(b+0xc0)], &C[(b+0xc0)] to sp+16/24/32, while (a+0xc0)/(b+0x40) are computed in the loop. Tried: brute force of explicit pointer locals (256 combos, best = ca4/sbc/cbc explicit), a4 int var, short cx/cy (helps), int x/y, flags with/without -fno-strength-reduce. Sibling wip func_80052190 has the same problem. */
// FLAGS -O2 -G0 -fno-strength-reduce
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
extern int D_1F800078;
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

void FUN_800532fc(TObj *o)
{
    PolyFT4 *p;
    short *e;
    char *q;
    unsigned char *f;
    unsigned long xy;
    short cx, cy;
    int a, b;
    short dx, dy;
    int ndx, ndy, dx2, dy2;
    unsigned char w;
    unsigned short h;
    short *ca4, *sbc, *cbc;

    if (o->anim == 0) return;
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
    ca4 = &D_8007A3F0[(a + 0x40) & 0xff];
    sbc = &D_8007A5F0[(b + 0xc0) & 0xff];
    cbc = &D_8007A3F0[(b + 0xc0) & 0xff];
    e = (short *)(o->d3c + *(unsigned short *)o->anim * 4);
    q = (char *)(*(volatile int *)&o->d3c + e[1]);
    D_1F800078 = e[0];
    f = (unsigned char *)q + 0xc;
    do {
        int x = (signed char)f[2];
        int y = (signed char)f[3];
        p = DAT_1f800164;
        w = f[-2];
        h = f[-1];
        if (o->animFrame & 1) {
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
            p->y2 = cy + ((ndx * D_8007A3F0[a]) >> 12) + ((dy2 * *ca4) >> 12);
            p->x3 = cx + ((dx2 * D_8007A5F0[b]) >> 12) + ((dy2 * *sbc) >> 12);
            p->y3 = cy + ((dx2 * D_8007A3F0[b]) >> 12) + ((dy2 * *cbc) >> 12);
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
            p->y3 = cy + ((ndx * D_8007A3F0[a]) >> 12) + ((dy2 * *ca4) >> 12);
            p->x2 = cx + ((dx2 * D_8007A5F0[b]) >> 12) + ((dy2 * *sbc) >> 12);
            p->y2 = cy + ((dx2 * D_8007A3F0[b]) >> 12) + ((dy2 * *cbc) >> 12);
        }
        if (onscreen(p)) {
            p->code = 0x2d;
            SetSemiTrans(p, o->b0d >> 7);
            *(unsigned long *)&p->uv0 = *(unsigned long *)q;
            *(unsigned long *)&p->uv1 = *(unsigned long *)(f - 8);
            p->uv2 = *(unsigned short *)(f - 4);
            p->uv3 = *(unsigned short *)f;
            p->tpage += o->w1e;
            if (o->b0d & 1)
                p->clut = o->w08;
            if (o->b0b) {
                if (otadd((unsigned long *)p, DAT_1f8001e0 + 0x10, 0, o->b0f, 0x9000000)) goto next;
            } else {
                if (otadd((unsigned long *)p, DAT_1f8001e0 + 0x10, D_1F800074, o->b0f, 0x9000000)) goto next;
            }
            DAT_1f800164 = (PolyFT4 *)((char *)DAT_1f800164 + 0x28);
        }
    next:
        f += 0x10;
        D_1F800078--;
        q += 0x10;
    } while (D_1F800078 != 0);
}
