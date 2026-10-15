// FUNC 80052190 2012 MAIN0
// MATCHING 80052190 2012
// FLAGS -O2 -G0 -fno-strength-reduce
#include "spad.h"
/* Rotated FT4 sprite strip with per-object colour (MAIN0 twin of drawObjectSprite / drawObjectSpriteRotScaled family): macro loop
   body (SN/CS, short x/y, u/w, short A/B/C/D, normal case first since the game branches with bnez), colour stores
   written in full per branch (cross-jumped tail, constant in v0), SPAD.i74 read as an array element so its lw
   stays after the uv stores, and `e = (short *)o->d3c; e += ...` (multi-set pointer) so the d3c load is not
   boosted and sits at the top of the preheader. */
#include "TOBJ.H"
#include "gamestate.h"
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
extern C4C *TASK_C;
extern long LZ_FILE_CTRL;
extern long D_1F800074a[];
extern char D_1F8000C0b[];
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern void SetSemiTrans(void *, int);
extern short D_8007A5F0[];
static short D_8007A3F0[256] = {
    0, -0x64, -0xc8, -0x12d, -0x191, -0x1f5, -0x259, -0x2bc,
    -0x31f, -0x381, -0x3e3, -0x444, -0x4a5, -0x504, -0x563, -0x5c2,
    -0x61f, -0x67b, -0x6d7, -0x731, -0x78a, -0x7e2, -0x839, -0x88f,
    -0x8e3, -0x936, -0x987, -0x9d7, -0xa26, -0xa73, -0xabe, -0xb08,
    -0xb50, -0xb96, -0xbda, -0xc1d, -0xc5e, -0xc9d, -0xcd9, -0xd14,
    -0xd4d, -0xd84, -0xdb9, -0xdeb, -0xe1c, -0xe4a, -0xe76, -0xea0,
    -0xec8, -0xeed, -0xf10, -0xf31, -0xf4f, -0xf6b, -0xf85, -0xf9c,
    -0xfb1, -0xfc3, -0xfd3, -0xfe1, -0xfec, -0xff4, -0xffb, -0xffe,
    -0x1000, -0xffe, -0xffb, -0xff4, -0xfec, -0xfe1, -0xfd3, -0xfc3,
    -0xfb1, -0xf9c, -0xf85, -0xf6b, -0xf4f, -0xf31, -0xf10, -0xeed,
    -0xec8, -0xea0, -0xe76, -0xe4a, -0xe1c, -0xdeb, -0xdb9, -0xd84,
    -0xd4d, -0xd14, -0xcd9, -0xc9d, -0xc5e, -0xc1d, -0xbda, -0xb96,
    -0xb50, -0xb08, -0xabe, -0xa73, -0xa26, -0x9d7, -0x987, -0x936,
    -0x8e3, -0x88f, -0x839, -0x7e2, -0x78a, -0x731, -0x6d7, -0x67b,
    -0x61f, -0x5c2, -0x563, -0x504, -0x4a5, -0x444, -0x3e3, -0x381,
    -0x31f, -0x2bc, -0x259, -0x1f5, -0x191, -0x12d, -0xc8, -0x64,
    0, 0x64, 0xc8, 0x12d, 0x191, 0x1f5, 0x259, 0x2bc,
    0x31f, 0x381, 0x3e3, 0x444, 0x4a5, 0x504, 0x563, 0x5c2,
    0x61f, 0x67b, 0x6d7, 0x731, 0x78a, 0x7e2, 0x839, 0x88f,
    0x8e3, 0x936, 0x987, 0x9d7, 0xa26, 0xa73, 0xabe, 0xb08,
    0xb50, 0xb96, 0xbda, 0xc1d, 0xc5e, 0xc9d, 0xcd9, 0xd14,
    0xd4d, 0xd84, 0xdb9, 0xdeb, 0xe1c, 0xe4a, 0xe76, 0xea0,
    0xec8, 0xeed, 0xf10, 0xf31, 0xf4f, 0xf6b, 0xf85, 0xf9c,
    0xfb1, 0xfc3, 0xfd3, 0xfe1, 0xfec, 0xff4, 0xffb, 0xffe,
    0x1000, 0xffe, 0xffb, 0xff4, 0xfec, 0xfe1, 0xfd3, 0xfc3,
    0xfb1, 0xf9c, 0xf85, 0xf6b, 0xf4f, 0xf31, 0xf10, 0xeed,
    0xec8, 0xea0, 0xe76, 0xe4a, 0xe1c, 0xdeb, 0xdb9, 0xd84,
    0xd4d, 0xd14, 0xcd9, 0xc9d, 0xc5e, 0xc1d, 0xbda, 0xb96,
    0xb50, 0xb08, 0xabe, 0xa73, 0xa26, 0x9d7, 0x987, 0x936,
    0x8e3, 0x88f, 0x839, 0x7e2, 0x78a, 0x731, 0x6d7, 0x67b,
    0x61f, 0x5c2, 0x563, 0x504, 0x4a5, 0x444, 0x3e3, 0x381,
    0x31f, 0x2bc, 0x259, 0x1f5, 0x191, 0x12d, 0xc8, 0x64,
};

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

static __inline__ int proj(void)
{
    gte_ldv0(((short*)&SPAD.s60));
    gte_rtps();
    gte_stflg(&SPAD.i8c);
    if (SPAD.i8c < 0) return 1;
    gte_stsxy(&LZ_FILE_CTRL);
    gte_stszotz(&SPAD.i74);
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

void drawObjectSpritePartsRotated3D(TObj *o)
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

    ((short*)&SPAD.s60)[0] = o->a.p.whole;
    SPAD.s62 = o->y.p.whole;
    if ((*(int*)&GAME.selectedArea) == 0x10005 && TASK_C->w4c != 3)
        SPAD.s64 = o->b.p.whole >> 2;
    else
        SPAD.s64 = o->b.p.whole;
    SetRotMatrix(((char*)&SPAD.bc0));
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
