// r9 wip: score 19 (4 bytes short). Only diff: li a1,0x9000000 gets moved into the delay slot of the 'b0d & 1' branch; game has nop there and loads D_1F800070 first, then li, then lb b0f.
// b17: sched1 puts the li (otadd's e param) first in the block, reorg then steals it into the beqz slot. Tried: all 24 param orders,
// e as literal (43), b computed inside, signed char d param, volatile/array/literal/struct-literal D_1F800070 (volatile la form puts li after la),
// b33: all 120 orders of the 5 args as locals before the call: no change (19); extern volatile D_1F800070 (plain lui form): no change; inline returning short gives 14 but adds a wrong move v0,a0 (size then matches by accident); flags no-sched/-fno-sched2 worse.
// asm barrier at inline start, return form. The final bnez nop is fine (maspsx expands the lw macro).
// b40: sched1 places li late; sched2 (bottom-up) picks li last because lw D_1F800070 (prio 5) beats it; game needs lw c not ready/lower prio than li. Tried c/d param types (long/short/schar/uchar), d=(sc)d;d+=c;d<<=2, ==0/return/result-var call forms, volatile on c/b/b0f, D_1F800070[0]: all >=19.
// b48: sched2 -dR: block 274-297 roots lw c(287), lb d(294), lw b(283), li e(291) all prio 1; at the last pick {li, lw c} the load wins by 'greater potential hazard', so li ends first and reorg copies it into the beqz slot (label has 2 preds -> only the first insn of the target thread can be copied). Game needs lw c picked last, i.e. li with prio>=2 or lw c not ready.
// FUNC 80054c38 796 MAIN0
#include "TOBJ.H"
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
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
typedef struct {
    unsigned long w0, w4;
    unsigned short uv2, p0a;
    unsigned short uv3;
} Face;
extern MATRIX D_1F800000, D_1F8000C0;
extern SVECTOR D_1F800060;
extern long D_1F800070;
extern PolyFT4 *DAT_1f800164;
extern char *DAT_1f8001e0;
extern void FUN_80021f5c(MATRIX *);
extern void SetRotMatrix(MATRIX *);
extern void ApplyRotMatrix(SVECTOR *, long *);
extern void SetTransMatrix(MATRIX *);
extern void SetSemiTrans(PolyFT4 *, int);

#define gte_ldv3(r0, r1, r2) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 0( %1 );lwc2 $3, 4( %1 );lwc2 $4, 0( %2 );lwc2 $5, 4( %2 )" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_stsxy3_ft4(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 16( %0 );swc2 $14, 24( %0 )" : : "r"(r0) : "memory")
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_avsz4() __asm__ volatile ("nop;nop;.word 0x4b68002e")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")

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

void func_80054C38(TObj *o)
{
    PolyFT4 *p;
    Face *f;
    char *v0, *v1, *v2, *v3;

    FUN_80021f5c(&D_1F800000);
    D_1F800060.vx = o->a.p.whole;
    D_1F800060.vy = o->y.p.whole;
    D_1F800060.vz = o->b.p.whole;
    SetRotMatrix(&D_1F8000C0);
    ApplyRotMatrix(&D_1F800060, D_1F800000.t);
    D_1F800000.t[0] += D_1F8000C0.t[0];
    D_1F800000.t[1] += D_1F8000C0.t[1];
    D_1F800000.t[2] += D_1F8000C0.t[2];
    SetTransMatrix(&D_1F800000);
    v0 = (char *)o + 0xb4;
    v1 = (char *)o + 0xbc;
    v2 = (char *)o + 0xc4;
    v3 = (char *)o + 0xcc;
    p = DAT_1f800164;
    gte_ldv3(v0, v1, v2);
    gte_rtpt();
    gte_stsxy3_ft4(p);
    gte_ldv0(v3);
    gte_rtps();
    gte_avsz4();
    gte_stotz(&D_1F800070);
    gte_stsxy(&p->x3);
    if (onscreen(p)) {
        f = (Face *)(*(volatile int *)&o->d3c + ((short *)(o->d3c + (*(unsigned short *)o->anim << 2)))[1]);
        p->code = 0x2d;
        SetSemiTrans(p, o->b0d >> 7);
        *(unsigned long *)&p->uv0 = f->w0;
        *(unsigned long *)&p->uv1 = f->w4;
        p->uv2 = f->uv2;
        p->uv3 = f->uv3;
        p->tpage += o->w1e;
        if (o->b0d & 1)
            p->clut = o->w08;
        if (!otadd((unsigned long *)p, DAT_1f8001e0 + 0x10, D_1F800070, o->b0f, 0x9000000))
            DAT_1f800164 = (PolyFT4 *)((char *)DAT_1f800164 + 0x28);
    }
}
