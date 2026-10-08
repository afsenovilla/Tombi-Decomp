// FUNC 80056e4c 860 MAIN0
/* score 4 (b48): `one = 1; do { } while (0);` after the project() test puts li a1 in its own sched block (first insn after bnez -> reorg fills the slot, as in the game). Left: move a0,s2 fills the 2nd lhu-wb4 stall instead of the lhu-anim stall (it is ready as soon as the call is scheduled, prio 2). Tried: q = p copy + second empty loop after the lhu (CSE folds q), second loop before each setRGB store, volatile combos x statement orders (4 best). Older notes: */
/* score 6 (was 18): only SetSemiTrans arg setup differs: game has li a1,1 in the bnez delay slot and move a0,s2 right after lhu anim; ours gets li a1,1 after lhu (reorg cannot scan past volatile loads, so li must be scheduled before them or the game has no volatile). otadd sibling form (c=z, d=b0f) fixed the tail; tried volatility combos of all 6 loads, statement hill-climb, non-volatile reload forms (all CSEd). b43: `t = o->d3c; xy = ...; t += *a * 4; s = o->d3c;` keeps two d3c loads WITHOUT volatile (reassigning t breaks the CSE equivalence) and puts li a1 in the bnez slot, but load order then is anim,d3c,d3c,xy,lhu,p (best 15-31 over all volatile combos x statement perms). sched2 dump: game needs move a0,s2 (insn 152) still unscheduled when filling the lhu-anim stall; ours uses it to fill the lhu wb4 stall. b48: sched2 -dR shows move a0 (prio 2) and li a1 (prio 1) both ready right after the call is scheduled, so they fill the lhu-wb4 stalls; the game has nops there, i.e. neither was ready yet (some dependency we lack). Tried int one=1 var, old-style/short/uchar SetSemiTrans protos, inline wrappers around the call/setRGB block, block-local q=p copy: all 6; all-non-volatile load orders x b43 form best 31. */
#include "TOBJ.H"
typedef struct {
    unsigned long tag;
    unsigned char r0;
    unsigned char g0;
    unsigned char b0;
    unsigned char code;
    unsigned short x0;
    unsigned short y0;
    unsigned char u0;
    unsigned char v0;
    unsigned short clut;
    unsigned short x1;
    unsigned short y1;
    unsigned char u1;
    unsigned char v1;
    unsigned short tpage;
    unsigned short x2;
    unsigned short y2;
    unsigned char u2;
    unsigned char v2;
    unsigned short pad2;
    unsigned short x3;
    unsigned short y3;
    unsigned char u3;
    unsigned char v3;
    unsigned short pad3;
} PolyFT4;

typedef struct { short vx, vy, vz, pad; } SVec;

extern int D_8009C960;
extern SVec D_1F800060;
extern long D_1F80008C;
extern volatile long D_1F800070;
extern long D_1F800074;
extern char D_1F8000C0[];
extern char D_1F8000C0b[];
extern TObj *DAT_1f8001d4;
extern PolyFT4 *volatile DAT_1f800164;
extern int DAT_1f8001e0;
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern void SetSemiTrans(PolyFT4 *, int);

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

static __inline__ int project(void)
{
    gte_ldv0(&D_1F800060);
    gte_rtps();
    gte_stflg(&D_1F80008C);
    if (D_1F80008C < 0)
        return 1;
    gte_stsxy(&D_1F800070);
    gte_stszotz(&D_1F800074);
    return 0;
}

static __inline__ int addprim(unsigned *a, char *b, int c, int d, unsigned e)
{
    d = ((signed char)d + c) << 2;
    if (d < 0) d = 0;
    d += (int)b;
    if ((unsigned)(d - DAT_1f8001e0) >= 0xca0) return 1;
    {
        unsigned v = *(unsigned *)d;
        *(unsigned *)d = (unsigned)a;
        *a = v | e;
    }
    return 0;
}

void func_80056E4C(TObj *o)
{
    PolyFT4 *p;
    unsigned char *s;
    int xy;
    int t;
    int z;
    short *hh;
    unsigned short *a;
    unsigned char w;
    unsigned char h;
    short u; int one;
    unsigned char v;

    D_1F800060.vx = o->a.p.whole;
    D_1F800060.vy = o->y.p.whole;
    if (D_8009C960 == 0x10005 && *(unsigned short *)&DAT_1f8001d4->w4c != 3)
        D_1F800060.vz = o->b.p.whole >> 2;
    else
        D_1F800060.vz = o->b.p.whole;
    SetRotMatrix(D_1F8000C0);
    SetTransMatrix(D_1F8000C0b);
    if (project())
        return;
    one = 1;
    do { } while (0);
    a = *(unsigned short *volatile *)&o->anim;
    t = *(volatile int *)&o->d3c;
    xy = D_1F800070;
    s = (unsigned char *)*(volatile int *)&o->d3c;
    p = DAT_1f800164;
    hh = (short *)(t + *(volatile unsigned short *)a * 4);
    s += hh[1];
    p->code = 0x2c;
    p->r0 = o->wb4;
    p->g0 = o->wb4;
    p->b0 = o->wb4;
    SetSemiTrans(p, one);
    p->tpage = o->w1e;
    u = s[0];
    v = s[1];
    w = s[0xa];
    h = s[0xb];
    p->u0 = u;
    p->v0 = v;
    p->v1 = v;
    p->u1 = w + u;
    p->u2 = u;
    p->v2 = v + h;
    p->u3 = w + u;
    p->v3 = v + h;
    p->x0 = xy + ((signed char)s[0xe] * (unsigned short)o->wb8) / 200;
    p->y0 = (xy >> 16) + ((signed char)s[0xf] * (unsigned short)o->wb8) / 200;
    t = (w * (unsigned short)o->wb8) / 100;
    p->y1 = *(volatile unsigned short *)&p->y0;
    p->x2 = p->x0;
    p->x1 = p->x0 + t - 1;
    p->y2 = p->y0 + (h * (unsigned short)o->wb8) / 100 - 1;
    p->x3 = p->x1;
    p->y3 = p->y2;
    z = D_1F800074;
    p->clut = o->w08;
    if (addprim((unsigned *)p, (char *)(DAT_1f8001e0 + 0x10), z, o->b0f, 0x9000000) == 0)
        DAT_1f800164 = (PolyFT4 *)((char *)DAT_1f800164 + 0x28);
}
