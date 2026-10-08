// FUNC 8011dbf4 1904 X006
/* score 497: rotated FT4 sprite loop (same family as wips src/wip/FUN_800505f0.c and src/wip/FUN_80123fd0.c; tail is
   the MAIN0 FUN_80052db8 code). Logic decoded. Left: loop-invariant hoisting (game keeps sin/cos bases in s5/t5,
   &sin/&cos[a] t3/fp, &sin/&cos[b] s7/s6, a/b and the a+0x40 offset spilled at sp18/sp20/sp28, &cos[a+0x40]/
   &sin,&cos[b+0xc0] at sp30/38/40, q at sp10, t3-t5 caller-saved around the calls), x/y register swap (game s4=xy,
   s3=xy>>16). Tried: a4/b+0xc0 pointer locals (642 -> 493), volatile d3c reread, -fno-strength-reduce (worse). */
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
extern PolyFT4 *DAT_1f800164;
extern char *DAT_1f8001e0;
extern long D_1F800070;
extern long D_1F800074;
extern int D_1F800078;
extern short D_8007A5F0[];
extern short D_8007A3F0[];
extern int FUN_8004fba8(void *o, long *sxy, long *z);
extern int FUN_8004fcc0(void *p);
extern void SetSemiTrans(void *, int);

#define SN D_8007A5F0
#define CS D_8007A3F0

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

void func_8011DBF4(TObj *o)
{
    PolyFT4 *p;
    short *e;
    char *q;
    unsigned char *f;
    int x, y, k;
    int a, b;
    short A, B, C, D;
    int u, v;

    if (o->anim == 0) return;
    FUN_8004fba8(o, &D_1F800070, &D_1F800074);
    x = D_1F800070;
    y = (unsigned long)D_1F800070 >> 16;
    k = o->d64 >> 4;
    a = (o->d8c + 0x80) & 0xff;
    b = o->d8c & 0xff;
    e = (short *)(o->d3c + *(unsigned short *)o->anim * 4);
    q = (char *)(*(volatile int *)&o->d3c + e[1]);
    D_1F800078 = e[0];
    f = (unsigned char *)q + 0xc;
    {int a4 = (a + 0x40) & 0xff;
    short *sb = &SN[(b + 0xc0) & 0xff];
    short *cb = &CS[(b + 0xc0) & 0xff];
    do {
        A = (unsigned)((signed char)f[2] * k) >> 8;
        B = (unsigned)((signed char)f[3] * k) >> 8;
        C = (unsigned)(f[-2] * k) >> 8;
        D = (unsigned)(f[-1] * k) >> 8;
        p = DAT_1f800164;
        if (o->animFrame & 1) {
            u = -A;
            v = -B;
            p->x0 = x + ((u * SN[a]) >> 12) + ((v * SN[(a + 0xc0) & 0xff]) >> 12);
            p->y0 = y + ((u * CS[a]) >> 12) + ((v * CS[(a + 0xc0) & 0xff]) >> 12);
            A += C;
            p->x1 = x + ((A * SN[b]) >> 12) + ((v * SN[(b + 0x40) & 0xff]) >> 12);
            p->y1 = y + ((A * CS[b]) >> 12) + ((v * CS[(b + 0x40) & 0xff]) >> 12);
            B += D;
            p->x2 = x + ((u * SN[a]) >> 12) + ((B * SN[a4]) >> 12);
            p->y2 = y + ((u * CS[a]) >> 12) + ((B * CS[a4]) >> 12);
            p->x3 = x + ((A * SN[b]) >> 12) + ((B * *sb) >> 12);
            p->y3 = y + ((A * CS[b]) >> 12) + ((B * *cb) >> 12);
        } else {
            A = -(A + C);
            u = -A;
            v = -B;
            p->x1 = x + ((u * SN[a]) >> 12) + ((v * SN[(a + 0xc0) & 0xff]) >> 12);
            p->y1 = y + ((u * CS[a]) >> 12) + ((v * CS[(a + 0xc0) & 0xff]) >> 12);
            A += C;
            p->x0 = x + ((A * SN[b]) >> 12) + ((v * SN[(b + 0x40) & 0xff]) >> 12);
            p->y0 = y + ((A * CS[b]) >> 12) + ((v * CS[(b + 0x40) & 0xff]) >> 12);
            B += D;
            p->x3 = x + ((u * SN[a]) >> 12) + ((B * SN[a4]) >> 12);
            p->y3 = y + ((u * CS[a]) >> 12) + ((B * CS[a4]) >> 12);
            p->x2 = x + ((A * SN[b]) >> 12) + ((B * *sb) >> 12);
            p->y2 = y + ((A * CS[b]) >> 12) + ((B * *cb) >> 12);
        }
        if (FUN_8004fcc0(p)) {
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
        q += 0x10;
        f += 0x10;
    } while (--D_1F800078 != 0);
    }
}
