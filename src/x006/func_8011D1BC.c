// FUNC 8011d1bc 804 X006
// MATCHING 8011d1bc 804
// FLAGS -O2 -G0 -fno-strength-reduce
/* Whole function incl. csv pieces 8011D334/8011D3EC. Debt: -fno-strength-reduce removes an extra giv. */
#include "TOBJ.H"

typedef struct {
    int tag;
    unsigned char r, g, b, code;
    short x0, y0; unsigned short u0, clut;
    short x1, y1; unsigned short u1, tpage;
    short x2, y2; unsigned short u2, pad2;
    short x3, y3; unsigned short u3, pad3;
} PFT4;

extern long D_1F800070;
extern long D_1F800074;
extern char *D_1F800164;
extern int D_1F8001E0;
extern int FUN_8004fba8(void *o, long *sxy, long *z);
extern int FUN_8004fcc0(void *p);
extern void SetSemiTrans(void *p, int abe);

static __inline__ int otA(unsigned *a, char *b, int c, int d, unsigned e)
{
    d = (short)d << 2;
    if (d < 0) d = 0;
    d += (int)b;
    if ((unsigned)(d - D_1F8001E0) >= 0xca0) return 1;
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
    if ((unsigned)(c - D_1F8001E0) >= 0xca0) return 1;
    {
        unsigned v = *(unsigned *)c;
        *(unsigned *)c = (unsigned)a;
        *a = v | e;
    }
    return 0;
}

void func_8011D1BC(TObj *o)
{
    int s;
    int n;
    unsigned char *base, *q;
    short *t;
    PFT4 *p;
    int r;

    if (o->anim == 0) return;
    FUN_8004fba8(o, &D_1F800070, &D_1F800074);
    t = (short *)o->d3c;
    t = (short *)((char *)t + *(short *)o->anim * 4);
    s = o->d64 >> 4;
    n = t[0];
    base = (unsigned char *)(o->d3c + t[1]);
    q = base + 0xc;
    do {
        p = (PFT4 *)D_1F800164;
        if (o->animFrame & 1) {
            p->x0 = D_1F800070 + ((unsigned)(((signed char *)q)[2] * s) >> 8);
            p->y0 = (D_1F800070 >> 16) + ((unsigned)(((signed char *)q)[3] * s) >> 8);
            p->x1 = p->x0 + ((unsigned)(q[-2] * s) >> 8);
            p->y1 = p->y0;
            p->x2 = p->x0;
            p->y2 = p->y0 + ((unsigned)(q[-1] * s) >> 8);
            p->x3 = p->x1;
            p->y3 = p->y2;
        } else {
            p->x0 = D_1F800070 - ((unsigned)(((signed char *)q)[2] * s) >> 8);
            p->y0 = (D_1F800070 >> 16) + ((unsigned)(((signed char *)q)[3] * s) >> 8);
            p->x1 = p->x0 - ((unsigned)(q[-2] * s) >> 8);
            p->y1 = p->y0;
            p->x2 = p->x0;
            p->y2 = p->y0 + ((unsigned)(q[-1] * s) >> 8);
            p->x3 = p->x1;
            p->y3 = p->y2;
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
            if (o->b0b) r = otA((unsigned *)p, (char *)D_1F8001E0 + 0x10, 0, (signed char)o->b0f, 0x9000000);
            else r = otB((unsigned *)p, (char *)D_1F8001E0 + 0x10, D_1F800074, (signed char)o->b0f, 0x9000000);
            if (r == 0) D_1F800164 += 0x28;
        }
        q += 0x10;
        base += 0x10;
    } while (--n);
}
