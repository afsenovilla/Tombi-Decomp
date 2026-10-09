// FUNC 8011cef0 716 X006
/* score 45: in both animFrame branches the game loads e->w before the p->x2/p->y1 stores and reloads p->x0 after
   the x2 store (ours loads w after both stores); the second OT insert loads D_1F800074 before o->b0f.
   Tried: w in a local (x0 then CSEd, x2/y1 stores cross-jumped), raw/volatile accesses, statement hill-climb. */
#include "TOBJ.H"

typedef struct {
    unsigned int tag;
    unsigned char r, g, b, code;
    short x0, y0; unsigned short uv0, clut;
    short x1, y1; unsigned short uv1, tpage;
    short x2, y2; unsigned short uv2, pad2;
    short x3, y3; unsigned short uv3, pad3;
} FT4;

typedef struct {
    int uvclut;
    int uvtp;
    unsigned short uv2;
    unsigned char w, h;
    unsigned short uv3;
    signed char dx, dy;
} EL;

extern int D_1F800070, D_1F800074;
extern FT4 *D_1F800164;
extern int D_1F8001E0;
extern int FUN_8004fba8(void *, int *, int *);
extern int FUN_8004fcc0(void *);
extern void SetSemiTrans(void *, int);

static __inline__ int ot(unsigned *a, int b, int c, int z, unsigned e)
{
    int d = (c + z) << 2;
    if (d < 0) d = 0;
    d += b;
    if ((unsigned)(d - D_1F8001E0) >= 0xca0) return 1;
    {
        unsigned v = *(unsigned *)d;
        *(unsigned *)d = (unsigned)a;
        *a = v | e;
    }
    return 0;
}

void func_8011CEF0(TObj *o)
{
    short *f;
    int n;
    EL *e;
    FT4 *p;
    int r;

    if (o->anim == 0) return;
    FUN_8004fba8(o, &D_1F800070, &D_1F800074);
    f = (short *)(o->d3c + (*(short *)o->anim << 2));
    n = f[0];
    e = (EL *)(*(volatile int *)&o->d3c + f[1]);
    do {
        p = D_1F800164;
        if (o->animFrame & 1) {
            p->x0 = e->dx + D_1F800070;
            p->y0 = e->dy + (D_1F800070 >> 16);
            p->x2 = p->x0;
            p->y1 = p->y0;
            p->x1 = p->x0 + e->w;
        } else {
            p->x0 = D_1F800070 - e->dx;
            p->y0 = e->dy + (D_1F800070 >> 16);
            p->x2 = p->x0;
            p->y1 = p->y0;
            p->x1 = p->x0 - e->w;
        }
        p->y2 = p->y0 + e->h;
        p->x3 = p->x1;
        p->y3 = p->y2;
        if (FUN_8004fcc0(p)) {
            p->code = 0x2d;
            SetSemiTrans(p, o->b0d >> 7);
            *(int *)&p->uv0 = e->uvclut;
            *(int *)&p->uv1 = e->uvtp;
            p->uv2 = e->uv2;
            p->uv3 = e->uv3;
            p->tpage += o->w1e;
            if (o->b0d & 1) p->clut = o->w08;
            if (o->b0b) r = ot((unsigned *)p, D_1F8001E0 + 0x10, 0, (signed char)o->b0f, 0x9000000);
            else r = ot((unsigned *)p, D_1F8001E0 + 0x10, D_1F800074, (signed char)o->b0f, 0x9000000);
            if (!r) D_1F800164 = (FT4 *)((char *)D_1F800164 + 0x28);
        }
        e++;
    } while (--n);
}
