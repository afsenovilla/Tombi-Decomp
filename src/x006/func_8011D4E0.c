// FUNC 8011d4e0 1812 X006
// MATCHING 8011d4e0 1812
// FLAGS -O2 -G0 -fno-strength-reduce
#include "spad.h"
/* Rotated FT4 sprite strip; whole function incl. csv pieces D5F4/D7E0/D8C4/DAE0. Built from x006 func_8011D1BC
   (same loop/OT insert, base+q pointers) and x009 func_8011F240 (rotation formulas). */
#include "TOBJ.H"

typedef struct {
    int tag;
    unsigned char r, g, b, code;
    short x0, y0; unsigned short u0, clut;
    short x1, y1; unsigned short u1, tpage;
    short x2, y2; unsigned short u2, pad2;
    short x3, y3; unsigned short u3, pad3;
} PFT4;

extern long LZ_FILE_CTRL;
extern int D_1F800078;
extern char *D_1F800164;
extern short D_8007A5F0[];
extern short D_8007A3F0[];
extern int projectActorPosition(void *o, long *sxy, long *z);
extern int FUN_8004fcc0(void *p);
extern void SetSemiTrans(void *p, int abe);

#define SN(i) D_8007A5F0[i]
#define CS(i) D_8007A3F0[i]
#define a1 ((ang + 0x80) & 0xff)
#define a0 (ang & 0xff)

static __inline__ int otA(unsigned *a, char *b, int c, int d, unsigned e)
{
    d = (short)d << 2;
    if (d < 0) d = 0;
    d += (int)b;
    if ((unsigned)(d - SPAD.i1e0) >= 0xca0) return 1;
    {
        unsigned v = *(unsigned *)d;
        *(unsigned *)d = (unsigned)a;
        *a = v | e;
    }
    return 0;
}

static __inline__ int otB(unsigned *a, char *b, int c, int d, unsigned e)
{
    c = (d + c) << 2;
    if (c < 0) c = 0;
    c += (int)b;
    if ((unsigned)(c - SPAD.i1e0) >= 0xca0) return 1;
    {
        unsigned v = *(unsigned *)c;
        *(unsigned *)c = (unsigned)a;
        *a = v | e;
    }
    return 0;
}

void func_8011D4E0(TObj *o)
{
    int xy;
    short x, y;
    int ang;
    int d;
    short *t;
    unsigned char *base, *q;
    short A, B, C, D;
    short u, w;
    PFT4 *p;
    int r;

    if (o->anim == 0) return;
    projectActorPosition(o, &LZ_FILE_CTRL, &SPAD.i74);
    xy = LZ_FILE_CTRL;
    d = o->d3c;
    x = xy;
    y = (unsigned)xy >> 16;
    ang = o->d8c;
    d += *(unsigned short *)o->anim * 4;
    t = (short *)d;
    base = (unsigned char *)(o->d3c + t[1]);
    D_1F800078 = t[0];
    q = base + 0xc;
    do {
        A = ((signed char *)q)[2];
        B = ((signed char *)q)[3];
        p = (PFT4 *)D_1F800164;
        C = q[-2];
        D = q[-1];
        if (o->animFrame & 1) {
            u = A;
            w = B;
            p->x0 = x + ((-u * SN(a1)) >> 12) + ((-w * SN((a1 + 0xc0) & 0xff)) >> 12);
            p->y0 = y + ((-u * CS(a1)) >> 12) + ((-w * CS((a1 + 0xc0) & 0xff)) >> 12);
            p->x1 = x + (((C + u) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            p->y1 = y + (((C + u) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            p->x2 = x + ((-u * SN(a1)) >> 12) + (((D + w) * SN((a1 + 0x40) & 0xff)) >> 12);
            p->y2 = y + ((-u * CS(a1)) >> 12) + (((D + w) * CS((a1 + 0x40) & 0xff)) >> 12);
            p->x3 = x + (((C + u) * SN(a0)) >> 12) + (((D + w) * SN((a0 + 0xc0) & 0xff)) >> 12);
            p->y3 = y + (((C + u) * CS(a0)) >> 12) + (((D + w) * CS((a0 + 0xc0) & 0xff)) >> 12);
        } else {
            u = -(A + C);
            w = B;
            p->x1 = x + ((-u * SN(a1)) >> 12) + ((-w * SN((a1 + 0xc0) & 0xff)) >> 12);
            p->y1 = y + ((-u * CS(a1)) >> 12) + ((-w * CS((a1 + 0xc0) & 0xff)) >> 12);
            p->x0 = x + (((C + u) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            p->y0 = y + (((C + u) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            p->x3 = x + ((-u * SN(a1)) >> 12) + (((D + w) * SN((a1 + 0x40) & 0xff)) >> 12);
            p->y3 = y + ((-u * CS(a1)) >> 12) + (((D + w) * CS((a1 + 0x40) & 0xff)) >> 12);
            p->x2 = x + (((C + u) * SN(a0)) >> 12) + (((D + w) * SN((a0 + 0xc0) & 0xff)) >> 12);
            p->y2 = y + (((C + u) * CS(a0)) >> 12) + (((D + w) * CS((a0 + 0xc0) & 0xff)) >> 12);
        }
        if (FUN_8004fcc0(p)) {
            p->code = 0x2d;
            SetSemiTrans(p, o->b0d >> 7);
            *(int *)&p->u0 = *(int *)base;
            *(int *)&p->u1 = *(int *)(q - 8);
            p->u2 = *(unsigned short *)(q - 4);
            p->u3 = *(unsigned short *)q;
            p->tpage += o->w1e;
            if (o->b0d & 1) p->clut = o->w08;
            if (o->b0b) r = otA((unsigned *)p, (char *)SPAD.i1e0 + 0x10, 0, (signed char)o->b0f, 0x9000000);
            else r = otB((unsigned *)p, (char *)SPAD.i1e0 + 0x10, SPAD.i74, (signed char)o->b0f, 0x9000000);
            if (r == 0) D_1F800164 += 0x28;
        }
        q += 0x10;
        base += 0x10;
    } while (--D_1F800078);
}
