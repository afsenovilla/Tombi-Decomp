// FUNC 80124780 772 X000
// MATCHING 80124780 772
#include "TOBJ.H"

typedef struct {
    TObj t;
    short wc0, wc2, wc4, wc6, wc8, wca, wcc, wce;
} Ext;

typedef struct {
    int tag;
    unsigned char r, g, b, code;
    short x0, y0; int uv0;
    short x1, y1; int uv1;
    short x2, y2; int uv2;
    short x3, y3; int uv3;
} PFT4;

typedef struct {
    int tag;
    unsigned char r, g, b, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} PF4;

extern long D_1F800070;
extern long D_1F800074;
extern char *D_1F800164;
extern int D_1F8001E0;
extern int D_1F800070i;
extern int D_1F800074i;
int FUN_8004fba8(void *o, long *sxy, long *z);
int FUN_8004fcc0(void *p);
void FUN_8004fe1c(void *o, void *p, void *q);
int FUN_8004fdc8(void *a, int b, int c, int d, unsigned e);
int FUN_8004fd6c(void *a, int b, int c, int d, unsigned e);

void func_80124780(Ext *o)
{
    PFT4 *p = (PFT4 *)D_1F800164;
    int xy;
    short x, y;
    PF4 *f;
    char *q;

    if (FUN_8004fba8(o, &D_1F800070, &D_1F800074) != 0)
        return;
    xy = D_1F800070i;
    x = xy;
    y = xy >> 16;
    p->x0 = x + o->t.wb4;
    p->y0 = y + o->t.wb6;
    p->x1 = x + o->t.wbc;
    p->y1 = y + *(short *)&o->t.bbe;
    p->x2 = x + o->wc4;
    p->y2 = y + o->wc6;
    p->x3 = x + o->wcc;
    p->y3 = y + o->wce;
    if (FUN_8004fcc0(p) == 0)
        return;
    FUN_8004fe1c(o, p, (char *)*(volatile int *)&o->t.d3c + *(short *)(o->t.d3c + (*(unsigned short *)o->t.anim << 2) + 2));
    if (o->t.b0b != 0)
        FUN_8004fdc8(p, D_1F8001E0 + 0x10, D_1F800074i, (signed char)o->t.b0f, 0x9000000);
    else
        FUN_8004fd6c(p, D_1F8001E0 + 0x10, D_1F800074i, (signed char)o->t.b0f, 0x9000000);
    q = D_1F800164;
    f = (PF4 *)(q + 0x28);
    D_1F800164 = (char *)f;
    if (o->t._pad0e[0] != 1)
        return;
    f->code = 0x29;
    f->r = 0xff;
    f->g = 0xff;
    f->b = 0xff;
    f->code &= ~2;
    *(int *)&f->x0 = *(int *)&p->x1;
    *(int *)&f->x2 = *(int *)&p->x3;
    f->x1 = f->x0 + 0x168;
    f->y1 = f->y0;
    f->x3 = f->x2 + 0x168;
    f->y3 = f->y2;

    if (o->t.b0b != 0)
        FUN_8004fdc8(f, D_1F8001E0 + 0x10, D_1F800070i, (signed char)o->t.b0f, 0x5000000);
    else
        FUN_8004fd6c(f, D_1F8001E0 + 0x10, D_1F800070i, (signed char)o->t.b0f, 0x5000000);
    D_1F800164 += 0x18;
}

void func_80124A20(TObj *o, TObj *p)
{
    if (p->subtype == 1
        && (unsigned short)(o->d->p.whole - p->d->p.whole + 0x3c) < 0x79
        && (unsigned short)(o->h->p.whole - p->h->p.whole) < 0x5b
        && (unsigned short)(o->box2 + (o->y.p.whole - p->y.p.whole + 0x20)) <= o->box3 + 0x40)
        *(unsigned char *)&o->wa8 = 4;
}
