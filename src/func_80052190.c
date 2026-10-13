// FUNC 80052190 2012 MAIN0
// MATCHING 80052190 2012
// FLAGS -O2 -G0 -fno-strength-reduce
/* Rotated FT4 sprite strip with per-object colour (MAIN0 twin of func_8005296C / func_80053B5C family): macro loop
   body (SN/CS, short x/y, u/w, short A/B/C/D, normal case first since the game branches with bnez), colour stores
   written in full per branch (cross-jumped tail, constant in v0), D_1F800074 read as an array element so its lw
   stays after the uv stores, and `e = (short *)o->d3c; e += ...` (multi-set pointer) so the d3c load is not
   boosted and sits at the top of the preheader. */
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
extern C4C *TASK_C;
extern short D_1F800060[];
extern short D_1F800062, D_1F800064;
extern long D_1F80008C;
extern long LZ_FILE_CTRL;
extern long D_1F800074;
extern long D_1F800074a[];
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
    gte_stsxy(&LZ_FILE_CTRL);
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

#define SN(i) D_8007A5F0[i]
#define a1 ((ang + 0x80) & 0xff)
#define a0 (ang & 0xff)
#define CS(i) D_8007A3F0[i]

void func_80052190(TObj *o)
{
    PolyFT4 *p;
    short *e;
    int n;
    unsigned char *q;
    unsigned char *f;
    int xy;
    short x, y;
    int ang;
    short A, B;
    short C, D;
    short u, w;

    D_1F800060[0] = o->a.p.whole;
    D_1F800062 = o->y.p.whole;
    if (D_8009C960 == 0x10005 && TASK_C->w4c != 3)
        D_1F800064 = o->b.p.whole >> 2;
    else
        D_1F800064 = o->b.p.whole;
    SetRotMatrix(D_1F8000C0);
    SetTransMatrix(D_1F8000C0b);
    if (proj()) return;
    xy = LZ_FILE_CTRL;
    e = (short *)o->d3c;
    e += *(unsigned short *)o->anim * 2;
    q = (unsigned char *)(*(volatile int *)&o->d3c + e[1]);
    n = e[0];
    x = xy;
    y = (unsigned)xy >> 16;
    ang = o->d8c;
    f = q + 0xc;
    do {
        A = (signed char)f[2];
        B = (signed char)f[3];
        p = DAT_1f800164;
        C = f[-2];
        D = f[-1];
        if ((o->animFrame & 1) == 0) {
            u = A;
            w = B;
            p->x0 = x + ((-u * SN(a1)) >> 12) + ((-w * SN((a1 + 0xc0) & 0xff)) >> 12);
            p->y0 = y + ((-u * CS(a1)) >> 12) + ((-w * CS((a1 + 0xc0) & 0xff)) >> 12);
            p->x1 = x + (((u + C) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            p->y1 = y + (((u + C) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            p->x2 = x + ((-u * SN(a1)) >> 12) + (((w + D) * SN((a1 + 0x40) & 0xff)) >> 12);
            p->y2 = y + ((-u * CS(a1)) >> 12) + (((w + D) * CS((a1 + 0x40) & 0xff)) >> 12);
            p->x3 = x + (((u + C) * SN(a0)) >> 12) + (((w + D) * SN((a0 + 0xc0) & 0xff)) >> 12);
            p->y3 = y + (((u + C) * CS(a0)) >> 12) + (((w + D) * CS((a0 + 0xc0) & 0xff)) >> 12);
        } else {
            u = -(A + C);
            w = B;
            p->x1 = x + ((-u * SN(a1)) >> 12) + ((-w * SN((a1 + 0xc0) & 0xff)) >> 12);
            p->y1 = y + ((-u * CS(a1)) >> 12) + ((-w * CS((a1 + 0xc0) & 0xff)) >> 12);
            p->x0 = x + (((u + C) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            p->y0 = y + (((u + C) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            p->x3 = x + ((-u * SN(a1)) >> 12) + (((w + D) * SN((a1 + 0x40) & 0xff)) >> 12);
            p->y3 = y + ((-u * CS(a1)) >> 12) + (((w + D) * CS((a1 + 0x40) & 0xff)) >> 12);
            p->x2 = x + (((u + C) * SN(a0)) >> 12) + (((w + D) * SN((a0 + 0xc0) & 0xff)) >> 12);
            p->y2 = y + (((u + C) * CS(a0)) >> 12) + (((w + D) * CS((a0 + 0xc0) & 0xff)) >> 12);
        }
        p->code = 0x2c;
        if (o->subtype == 0)
            SetSemiTrans(p, 1);
        else
            SetSemiTrans(p, 0);
        p->tpage = o->w1e;
        p->clut = o->w08;
        if (o->subtype == 0) {
            if (o->b0d & 0x40) {
                p->r0 = 0x20;
                p->g0 = 0x20;
                p->b0 = 0x20;
            } else {
                p->r0 = 0x40;
                p->g0 = 0x40;
                p->b0 = 0x40;
            }
        } else {
            p->r0 = 0x80;
            p->g0 = 0x80;
            p->b0 = 0x80;
        }
        p->uv0 = *(unsigned short *)q + 0x80;
        p->uv1 = *(unsigned short *)(f - 8) + 0x80;
        p->uv2 = *(unsigned short *)(f - 4) + 0x80;
        p->uv3 = *(unsigned short *)f + 0x80;
        if (!otadd((unsigned long *)p, DAT_1f8001e0 + 0x10, D_1F800074a[0], o->b0f, 0x9000000))
            DAT_1f800164 = (PolyFT4 *)((char *)DAT_1f800164 + 0x28);
        q += 0x10;
        f += 0x10;
    } while (--n);
}
