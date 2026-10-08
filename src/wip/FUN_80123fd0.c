// FUNC 80123fd0 1968 X000
/* score 610: logic decoded (4 rotated corners via sin/cos tables, mirrored when animFrame bit0 clear); game has 0x90 frame with more spilled invariants (sp 0x20..0x58), ours 0x70. d3c double read and loop-invariant hoisting not matched yet. */
#include "TOBJ.H"

typedef struct {
    int tag;
    unsigned char r, g, b, code;
    short x0, y0; unsigned char u0, v0; unsigned short clut;
    short x1, y1; unsigned char u1, v1; unsigned short tpage;
    short x2, y2; unsigned char u2, v2; unsigned short pad1;
    short x3, y3; unsigned char u3, v3; unsigned short pad2;
} PFT4_80123FD0;

extern long D_1F800070;
extern long D_1F800074;
extern int D_1F800070i;
extern int D_1F800074i;
extern PFT4_80123FD0 *D_1F800164;
extern int D_1F8001E0;
extern short D_1F8001C8;
extern short D_1F8000F6;
extern short D_1F8000EE;
extern short D_8007A5F0[];
extern short D_8007A3F0[];
int FUN_8004fba8(void *o, long *sxy, long *z);
int FUN_8004fcc0(void *p);
int FUN_8004fdc8(void *a, int b, int c, int d, unsigned e);
int FUN_8004fd6c(void *a, int b, int c, int d, unsigned e);

#define SN(i) D_8007A5F0[i]
#define CS(i) D_8007A3F0[i]

void FUN_80123fd0(TObj *o)
{
    short *e;
    unsigned char *q;
    int n, xy, x, y, s;
    int a1, a0;
    short A, B, C, D;
    short u0, v0, u1, v1;
    PFT4_80123FD0 *f;

    if (FUN_8004fba8(o, &D_1F800070, &D_1F800074) != 0)
        return;
    xy = D_1F800070i;
    y = (unsigned)xy >> 16;
    x = xy;
    e = (short *)(o->d3c + *(unsigned short *)o->anim * 4);
    n = e[0];
    q = (unsigned char *)(o->d3c + e[1]);
    switch (D_1F8001C8) {
    case 0:
        s = (D_1F8000F6 - o->b.p.whole) * 7 + 0x1000;
        break;
    case 1:
        s = (o->a.p.whole - D_1F8000EE) * 7 + 0x1000;
        break;
    }
    a1 = (o->d8c + 0x80) & 0xff;
    a0 = o->d8c & 0xff;
    do {
        A = ((signed char)q[14] * s - 0x1800) >> 12;
        B = ((signed char)q[15] * s - 0x1800) >> 12;
        C = (q[10] * s + 0x1800) >> 12;
        D = (q[11] * s + 0x1800) >> 12;
        f = D_1F800164;
        if (o->animFrame & 1) {
            u0 = -A;
            v0 = -B;
            u1 = A + C;
            v1 = B + D;
            f->x0 = x + ((u0 * SN(a1)) >> 12) + ((v0 * SN((a1 + 0xc0) & 0xff)) >> 12);
            f->y0 = y + ((u0 * CS(a1)) >> 12) + ((v0 * CS((a1 + 0xc0) & 0xff)) >> 12);
            f->x1 = x + ((u1 * SN(a0)) >> 12) + ((v0 * SN((a0 + 0x40) & 0xff)) >> 12);
            f->y1 = y + ((u1 * CS(a0)) >> 12) + ((v0 * CS((a0 + 0x40) & 0xff)) >> 12);
            f->x2 = x + ((u0 * SN(a1)) >> 12) + ((v1 * SN((a1 + 0x40) & 0xff)) >> 12);
            f->y2 = y + ((u0 * CS(a1)) >> 12) + ((v1 * CS((a1 + 0x40) & 0xff)) >> 12);
            f->x3 = x + ((u1 * SN(a0)) >> 12) + ((v1 * SN((a0 + 0xc0) & 0xff)) >> 12);
            f->y3 = y + ((u1 * CS(a0)) >> 12) + ((v1 * CS((a0 + 0xc0) & 0xff)) >> 12);
        } else {
            u0 = A + C;
            v0 = -B;
            u1 = -A;
            v1 = B + D;
            f->x1 = x + ((u0 * SN(a1)) >> 12) + ((v0 * SN((a1 + 0xc0) & 0xff)) >> 12);
            f->y1 = y + ((u0 * CS(a1)) >> 12) + ((v0 * CS((a1 + 0xc0) & 0xff)) >> 12);
            f->x0 = x + ((u1 * SN(a0)) >> 12) + ((v0 * SN((a0 + 0x40) & 0xff)) >> 12);
            f->y0 = y + ((u1 * CS(a0)) >> 12) + ((v0 * CS((a0 + 0x40) & 0xff)) >> 12);
            f->x3 = x + ((u0 * SN(a1)) >> 12) + ((v1 * SN((a1 + 0x40) & 0xff)) >> 12);
            f->y3 = y + ((u0 * CS(a1)) >> 12) + ((v1 * CS((a1 + 0x40) & 0xff)) >> 12);
            f->x2 = x + ((u1 * SN(a0)) >> 12) + ((v1 * SN((a0 + 0xc0) & 0xff)) >> 12);
            f->y2 = y + ((u1 * CS(a0)) >> 12) + ((v1 * CS((a0 + 0xc0) & 0xff)) >> 12);
        }
        if (FUN_8004fcc0(f)) {
            f->code = 0x2d;
            if (o->b0d >> 7)
                f->code = 0x2f;
            f->v0 = 0xa0;
            f->u0 = ((unsigned char *)o)[0xc6] << 7;
            f->v1 = 0xa0;
            f->u1 = q[10] + (((unsigned char *)o)[0xc6] << 7);
            f->u2 = ((unsigned char *)o)[0xc6] << 7;
            f->v2 = q[11] + 0xa0;
            f->u3 = q[10] + (((unsigned char *)o)[0xc6] << 7);
            f->v3 = q[11] + 0xa0;
            f->tpage = o->w1e;
            f->clut = o->w08;
            if (o->b0b != 0)
                FUN_8004fdc8(f, D_1F8001E0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
            else
                FUN_8004fd6c(f, D_1F8001E0 + 0x10, D_1F800074i, (signed char)o->b0f, 0x9000000);
            D_1F800164 = (PFT4_80123FD0 *)((char *)D_1F800164 + 0x28);
        }
        q += 16;
    } while (--n);
}
