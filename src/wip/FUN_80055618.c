// FUNC 80055618 1696 MAIN0
/* score 28: (1) game loads o->d3c (first, for e) at the top of the if block, ours right before use (volatile/nonvolatile/inline/order variants all tried); (2) addu u+w scheduled before sb u0 in ours. */
#include "TOBJ.H"
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    unsigned short x0, y0;
    unsigned char u0, v0; unsigned short clut;
    unsigned short x1, y1;
    unsigned char u1, v1; unsigned short tpage;
    unsigned short x2, y2;
    unsigned char u2, v2; unsigned short pad2;
    unsigned short x3, y3;
    unsigned char u3, v3; unsigned short pad3;
} PolyFT4;
typedef struct { short n, p2, x0, y0, x1, y1; char pad[8]; } ENT;
typedef struct { ENT e[7]; } ROW;
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
extern ROW D_800a3fe0[];
extern short **D_800a4468;
extern short *D_800b0bb0;
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern void SetSemiTrans(void *, int);

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

#define WC6(o) (*(unsigned short *)((char *)(o) + 0xc6))

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

void FUN_80055618(TObj *o)
{
    PolyFT4 *p;
    unsigned char *q;
    int i;
    short *e;
    int d;
    ROW *tbl;
    short x;
    int y;
    unsigned long xy;
    int u;
    int v;
    unsigned char w;
    int h;

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
    x = xy;
    y = xy >> 16;
    tbl = D_800a3fe0;
    for (i = 0; i < 6; i++) {
        if ((&tbl[WC6(o)].e[i])->n > 0) {
            e = (short *)(*(volatile int *)&o->d3c + *D_800a4468[((unsigned short *)&o->wb4)[i]] * 4);
            p = DAT_1f800164;
            q = (unsigned char *)(*(volatile int *)&o->d3c + e[1]);
            p->code = 0x2d;
            SetSemiTrans(p, 0);
            u = q[0];
            v = q[1];
            w = q[10];
            h = q[11];
            p->u0 = u;
            p->u2 = u;
            p->v0 = v;
            p->u1 = u + w;
            p->v1 = v;
            p->v2 = v + h;
            p->u3 = u + w;
            p->v3 = v + h;
            p->x0 = x + (&tbl[WC6(o)].e[i])->x0 / 100;
            p->y0 = y + (&tbl[WC6(o)].e[i])->y0 / 100;
            p->x1 = x + (&tbl[WC6(o)].e[i])->x1 / 100;
            p->y1 = p->y0;
            p->x2 = p->x0;
            p->y2 = y + (&tbl[WC6(o)].e[i])->y1 / 100;
            p->x3 = p->x1;
            p->y3 = p->y2;
            p->tpage = o->w1e;
            p->clut = *(unsigned short *)(q + 2);
            if (!otadd((unsigned long *)p, DAT_1f8001e0, 0, o->b0f, 0x9000000))
                DAT_1f800164 = (PolyFT4 *)((char *)DAT_1f800164 + 0x28);
        }
    }
    e = (short *)(*(volatile int *)&o->d3c + *D_800b0bb0 * 4);
    q = (unsigned char *)(*(volatile int *)&o->d3c + e[1]);
    p = DAT_1f800164;
    p->code = 0x2d;
    SetSemiTrans(p, 0);
    u = q[0];
    v = q[1];
    w = q[10];
    h = q[11];
    p->u0 = u;
    p->u2 = u;
    p->v0 = v;
    p->u1 = u + w;
    p->v1 = v;
    p->v2 = v + h;
    p->u3 = u + w;
    p->v3 = v + h;
    p->x0 = x + D_800a3fe0[WC6(o)].e[6].x0 / 100;
    p->y0 = y + D_800a3fe0[WC6(o)].e[6].y0 / 100;
    p->x1 = x + D_800a3fe0[WC6(o)].e[6].x1 / 100;
    p->y1 = p->y0;
    p->x2 = p->x0;
    p->y2 = y + D_800a3fe0[WC6(o)].e[6].y1 / 100;
    p->x3 = p->x1;
    p->y3 = p->y2;
    p->tpage = o->w1e;
    p->clut = *(unsigned short *)(q + 2);
    if (!otadd((unsigned long *)p, DAT_1f8001e0, 0, o->b0f, 0x9000000))
        DAT_1f800164 = (PolyFT4 *)((char *)DAT_1f800164 + 0x28);
}
