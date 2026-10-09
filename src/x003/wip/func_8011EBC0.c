// FUNC 8011ebc0 1872 X003
/* score 625: adapted from the X014 twin-like wip func_8011C00C (same renderer: byte scale without ba7, code 0x2d/0x2f, OT call result ignored, no rgb copy, else branch = A = -(A + T2) with mirrored stores). Same open problem as X014: loop-invariant hoisting of the sin/cos table pointers and spills (game: S in s5, C in t4 spilled around calls, offsets at sp+0x30..0x60), frame 0x98. Not tuned. */
#include "TOBJ.H"

typedef struct {
    int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0; int uv0;
    short x1, y1; int uv1;
    short x2, y2; unsigned short uv2, pad2;
    short x3, y3; unsigned short uv3, pad3;
} PFT4;

extern long D_1F800070;
extern long D_1F800074;
extern int D_1F800070i;
extern int D_1F800074i;
extern int D_1F800078;
extern char *D_1F800164;
extern int D_1F8001E0;
extern short D_8007A5F0[];
extern short D_8007A3F0[];
int FUN_8004fba8(void *o, long *sxy, long *z);
int FUN_8004fcc0(void *p);
int FUN_8004fdc8(void *a, int b, int c, int d, unsigned e);
int FUN_8004fd6c(void *a, int b, int c, int d, unsigned e);

void func_8011EBC0(TObj *o)
{
    PFT4 *p;
    unsigned char *e;
    unsigned char *q;
    int xy;
    int x;
    unsigned int y;
    int a, b;
    short *fr;
    unsigned int s;
    short A, T2, V, T3;
    int c;

    if (o->anim == 0) return;
    FUN_8004fba8(o, &D_1F800070, &D_1F800074);
    xy = D_1F800070i;
    x = xy;
    y = (unsigned int)xy >> 16;
    a = (o->d8c + 0x80) & 0xff;
    b = o->d8c & 0xff;
    fr = (short *)(o->d3c + (*(unsigned short *)o->anim << 2));
    q = (unsigned char *)(*(volatile int *)&o->d3c + fr[1]);
    e = q + 0xc;
    D_1F800078 = fr[0];
    do {
        p = (PFT4 *)D_1F800164;
        c = (signed char)o->ba5;
        if (c != 0) {
            s = (unsigned char)c;
            A = (unsigned)(((signed char *)e)[2] * s) >> 8;
            T2 = (unsigned)(e[-2] * s) >> 8;
        } else {
            A = (signed char)e[2];
            T2 = e[-2];
        }
        c = (signed char)o->ba6;
        if (c != 0) {
            s = (unsigned char)c;
            V = (unsigned)(((signed char *)e)[3] * s) >> 8;
            T3 = (unsigned)(e[-1] * s) >> 8;
        } else {
            V = (signed char)e[3];
            T3 = e[-1];
        }
        if (o->animFrame & 1) {
            p->x0 = x + ((-A * D_8007A5F0[a]) >> 12) + ((-V * D_8007A5F0[(a + 0xc0) & 0xff]) >> 12);
            p->y0 = y + ((-A * D_8007A3F0[a]) >> 12) + ((-V * D_8007A3F0[(a + 0xc0) & 0xff]) >> 12);
            p->x1 = x + (((A + T2) * D_8007A5F0[b]) >> 12) + ((-V * D_8007A5F0[(b + 0x40) & 0xff]) >> 12);
            p->y1 = y + (((A + T2) * D_8007A3F0[b]) >> 12) + ((-V * D_8007A3F0[(b + 0x40) & 0xff]) >> 12);
            p->x2 = x + ((-A * D_8007A5F0[a]) >> 12) + (((V + T3) * D_8007A5F0[(a + 0x40) & 0xff]) >> 12);
            p->y2 = y + ((-A * D_8007A3F0[a]) >> 12) + (((V + T3) * D_8007A3F0[(a + 0x40) & 0xff]) >> 12);
            p->x3 = x + (((A + T2) * D_8007A5F0[b]) >> 12) + (((V + T3) * D_8007A5F0[(b + 0xc0) & 0xff]) >> 12);
            p->y3 = y + (((A + T2) * D_8007A3F0[b]) >> 12) + (((V + T3) * D_8007A3F0[(b + 0xc0) & 0xff]) >> 12);
        } else {
            A = -(A + T2);
            p->x1 = x + ((-A * D_8007A5F0[a]) >> 12) + ((-V * D_8007A5F0[(a + 0xc0) & 0xff]) >> 12);
            p->y1 = y + ((-A * D_8007A3F0[a]) >> 12) + ((-V * D_8007A3F0[(a + 0xc0) & 0xff]) >> 12);
            p->x0 = x + (((A + T2) * D_8007A5F0[b]) >> 12) + ((-V * D_8007A5F0[(b + 0x40) & 0xff]) >> 12);
            p->y0 = y + (((A + T2) * D_8007A3F0[b]) >> 12) + ((-V * D_8007A3F0[(b + 0x40) & 0xff]) >> 12);
            p->x3 = x + ((-A * D_8007A5F0[a]) >> 12) + (((V + T3) * D_8007A5F0[(a + 0x40) & 0xff]) >> 12);
            p->y3 = y + ((-A * D_8007A3F0[a]) >> 12) + (((V + T3) * D_8007A3F0[(a + 0x40) & 0xff]) >> 12);
            p->x2 = x + (((A + T2) * D_8007A5F0[b]) >> 12) + (((V + T3) * D_8007A5F0[(b + 0xc0) & 0xff]) >> 12);
            p->y2 = y + (((A + T2) * D_8007A3F0[b]) >> 12) + (((V + T3) * D_8007A3F0[(b + 0xc0) & 0xff]) >> 12);
        }
        if (FUN_8004fcc0(p) == 0) goto next;
        p->code = 0x2d;
        if (o->b0d >> 7) p->code = 0x2f;
        p->uv0 = *(int *)q;
        p->uv1 = *(int *)(e - 8);
        p->uv2 = *(unsigned short *)(e - 4);
        p->uv3 = *(unsigned short *)e;
        *(short *)((char *)p + 0x16) += o->w1e;
        if (o->b0d & 1) *(short *)((char *)p + 0xe) = o->w08;
        if (o->b0b != 0) {
            FUN_8004fdc8(p, D_1F8001E0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
        } else {
            FUN_8004fd6c(p, D_1F8001E0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
        }
        D_1F800164 += 0x28;
    next:
        q += 0x10;
        e += 0x10;
    } while (--D_1F800078 != 0);
}
