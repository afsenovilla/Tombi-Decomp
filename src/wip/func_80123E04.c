// FUNC 80123e04 460 X000
// 4 words differ: xy + (signed char)field comes out as field + xy (operand swap, see guide)
#include "TOBJ.H"
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0; unsigned short uv0, clut;
    short x1, y1; unsigned short uv1, tpage;
    short x2, y2; unsigned short uv2, pad1;
    short x3, y3; unsigned short uv3, pad2;
} PFT4;
typedef struct { unsigned short uv0, p0, uv1, p1, uv2; unsigned char w, h; unsigned short uv3; signed char dx, dy; } Spr;
extern char D_1F800070[];
extern char D_1F800074[];
extern PFT4 *D_1F800164;
extern int D_1F800074_v;
extern int D_1F800070_v;
extern int D_1F8001E0;
int FUN_8004fba8(TObj *o, void *a, void *b);
void FUN_8004fd6c(PFT4 *p, int a, int b, int c, int d);
void func_80123E04(TObj *o)
{
    Spr *s;
    PFT4 *p;
    int xy;
    s = (Spr *)(*(volatile int *)&o->d3c + ((short *)((char *)o->d3c + (*(unsigned short *)o->anim << 2)))[1]);
    if (FUN_8004fba8(o, D_1F800070, D_1F800074) != 0) return;
    xy = D_1F800070_v;
    p = D_1F800164;
    p->code = 0x2c;
    p->r0 = o->wb6;
    p->g0 = o->wb6;
    p->b0 = o->wb6;
    p->code |= 2;
    p->uv0 = s->uv0;
    p->uv1 = s->uv1;
    p->uv2 = s->uv2;
    p->uv3 = s->uv3;
    p->tpage = o->w1e;
    p->clut = o->w08;
    p->x0 = xy + s->dx - o->wb8;
    p->y0 = (xy >> 16) + s->dy - o->wba;
    p->x1 = p->x0 + s->w + (unsigned short)o->wb8 * 2 - 1;
    p->y1 = p->y0;
    p->x2 = p->x0;
    p->y2 = p->y0 + s->h + (unsigned short)o->wba * 2 - 1;
    p->x3 = p->x1;
    p->y3 = p->y2;
    FUN_8004fd6c(p, D_1F8001E0 + 0x10, D_1F800074_v, (signed char)o->b0f, 0x9000000);
    D_1F800164 = (PFT4 *)((char *)D_1F800164 + 0x28);
}
