// FUNC 80123fd0 1968 X000
/* score 83 (was 610): b56 rewrite. Corner values u=A / u=-(A+C) as short with -u, u+C, w+D terms; angles written as
   macros on one int ang = o->d8c ((ang+0x80)&0xff, ang&0xff) so loop.c hoists exactly the game's invariants (frame 0x90);
   unsigned s gives the srl; d = o->d3c; d += idx*4 keeps the d3c reload; setUV order u then v.
   Left: (1) xy/x copy: game loads xy into a0, srl s4 / move s3 later; ours loads straight into s3. `short x` fixes the top
   exactly but then &CS[a1] (prio 27/295) beats s (30/328) for fp and s is spilled (127); needs +2 RTL insns inside the
   loop (or -1 between switch and loop) to flip; `short n` flips it (20) but n must be int (lw/sw).
   (2) li 0x2d lands in v0, game v1. Tried: callee proto types, code |= forms, temps, statement forms in loop. */
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
#define a1 ((ang + 0x80) & 0xff)
#define a0 (ang & 0xff)
#define CS(i) D_8007A3F0[i]

void FUN_80123fd0(TObj *o)
{
    short *e;
    unsigned char *q;
    int n;
    int xy;
    int x;
    short y;
    unsigned int s;
    int d;
    int ang;
    short A, B, C, D;
    short u, w;
    PFT4_80123FD0 *f;

    if (FUN_8004fba8(o, &D_1F800070, &D_1F800074) != 0)
        return;
    xy = D_1F800070i;
    x = xy;
    y = (unsigned)xy >> 16;
    d = o->d3c;
    d += *(unsigned short *)o->anim * 4;
    e = (short *)d;
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
    ang = o->d8c;
    do {
        A = ((signed char)q[14] * s - 0x1800) >> 12;
        B = ((signed char)q[15] * s - 0x1800) >> 12;
        C = (q[10] * s + 0x1800) >> 12;
        D = (q[11] * s + 0x1800) >> 12;
        f = D_1F800164;
        if (o->animFrame & 1) {
            u = A;
            w = B;
            f->x0 = x + ((-u * SN(a1)) >> 12) + ((-w * SN((a1 + 0xc0) & 0xff)) >> 12);
            f->y0 = y + ((-u * CS(a1)) >> 12) + ((-w * CS((a1 + 0xc0) & 0xff)) >> 12);
            f->x1 = x + (((u + C) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            f->y1 = y + (((u + C) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            f->x2 = x + ((-u * SN(a1)) >> 12) + (((w + D) * SN((a1 + 0x40) & 0xff)) >> 12);
            f->y2 = y + ((-u * CS(a1)) >> 12) + (((w + D) * CS((a1 + 0x40) & 0xff)) >> 12);
            f->x3 = x + (((u + C) * SN(a0)) >> 12) + (((w + D) * SN((a0 + 0xc0) & 0xff)) >> 12);
            f->y3 = y + (((u + C) * CS(a0)) >> 12) + (((w + D) * CS((a0 + 0xc0) & 0xff)) >> 12);
        } else {
            u = -(A + C);
            w = B;
            f->x1 = x + ((-u * SN(a1)) >> 12) + ((-w * SN((a1 + 0xc0) & 0xff)) >> 12);
            f->y1 = y + ((-u * CS(a1)) >> 12) + ((-w * CS((a1 + 0xc0) & 0xff)) >> 12);
            f->x0 = x + (((u + C) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            f->y0 = y + (((u + C) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            f->x3 = x + ((-u * SN(a1)) >> 12) + (((w + D) * SN((a1 + 0x40) & 0xff)) >> 12);
            f->y3 = y + ((-u * CS(a1)) >> 12) + (((w + D) * CS((a1 + 0x40) & 0xff)) >> 12);
            f->x2 = x + (((u + C) * SN(a0)) >> 12) + (((w + D) * SN((a0 + 0xc0) & 0xff)) >> 12);
            f->y2 = y + (((u + C) * CS(a0)) >> 12) + (((w + D) * CS((a0 + 0xc0) & 0xff)) >> 12);
        }
        if (FUN_8004fcc0(f)) {
            f->code = 0x2d;
            if (o->b0d >> 7)
                f->code = 0x2f;
            f->u0 = ((unsigned char *)o)[0xc6] << 7;
            f->v0 = 0xa0;
            f->u1 = q[10] + (((unsigned char *)o)[0xc6] << 7);
            f->v1 = 0xa0;
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
