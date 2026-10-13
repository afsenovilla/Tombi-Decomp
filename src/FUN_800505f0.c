// FUNC 800505f0 2228 MAIN0
// MATCHING 800505f0 2228
// FLAGS -O2 -G0 -fno-strength-reduce
#include "TOBJ.H"
#include "gamestate.h"
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
typedef struct { char pad[0x4c]; unsigned short w4c; } C4C;
extern PolyFT4 *DAT_1f800164;
extern char *DAT_1f8001e0;
extern C4C *TASK_C;
extern short D_1F800060[];
extern short D_1F800062, D_1F800064;
extern long D_1F80008C;
extern long LZ_FILE_CTRL;
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
#define C f[-1]
#define D f[0]

void FUN_800505f0(TObj *o)
{
    PolyFT4 *p;
    short *e;
    short n;
    unsigned char *q;
    unsigned char *f;
    short s;
    int xy;
    short x, y;
    int ang;
    short A, B;
    short u, w;
    unsigned short af;
    unsigned char c;

    D_1F800060[0] = o->a.p.whole;
    D_1F800062 = o->y.p.whole;
    if ((*(int*)&GAME.selectedArea) == 0x10005 && TASK_C->w4c != 3)
        D_1F800064 = o->b.p.whole >> 2;
    else
        D_1F800064 = o->b.p.whole;
    SetRotMatrix(D_1F8000C0);
    SetTransMatrix(D_1F8000C0b);
    if (proj()) return;
    s = *(short *)o->anim;
    xy = LZ_FILE_CTRL;
    e = (short *)o->d3c;
    e += s * 2;
    q = (unsigned char *)(*(volatile int *)&o->d3c + e[1]);
    n = ((unsigned short *)e)[0];
    x = xy;
    y = (unsigned)xy >> 16;
    if (D_8009C330->w28[D_1F8001F4] != s) {
        if (D_8009C960h == 6) {
            switch (D_8009C962h) {
            case 0: case 1: break;
            default: FUN_80037c80(o);
            }
        }
        FUN_80037c80(o);
    }
    ang = o->d8c;
    f = q + 0xb;
    D_8009C330->w18 = x;
    D_8009C330->w1a = y;
    do {
        A = (signed char)f[3];
        B = (signed char)f[4];
        p = DAT_1f800164;
        p->code = 0x2d;
        SetSemiTrans(p, 0);
        p->tpage = o->w1e;
        if (n & 2)
            p->clut = o->w08;
        else
            p->clut = *(unsigned short *)((char *)o + 0xc4);
        p->u0 = q[0] + ((unsigned char)D_1F8001F4 << 6) + 4;
        p->v0 = f[-10] + 4;
        c = C;
        p->u2 = p->u0;
        p->u1 = p->u0 + c;
        p->v1 = p->v0;
        p->v2 = p->v0 + D;
        p->u3 = p->u0 + C;
        p->v3 = p->v0 + D;
        if (o->b9d)
            af = D_8009C330->b7;
        else
            af = o->animFrame;
        if (af & 1) {
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
        } else {
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
        }
        {
            int c = D_1F800074;
            unsigned long e;
            do { } while (0); /* loop notes end the sched block after the c load (debt, as func_80054C38) */
            e = 0x9000000;
            if (!otadd((unsigned long *)p, DAT_1f8001e0 + 0x10, c, o->b0f, e))
                DAT_1f800164 = (PolyFT4 *)((char *)DAT_1f800164 + 0x28);
        }
        q += 0x10;
        f += 0x10;
    } while (--n);
}
