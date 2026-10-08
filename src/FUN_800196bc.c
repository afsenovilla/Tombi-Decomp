// FUNC 800196bc 504 MAIN0
// MATCHING 800196bc 504
typedef struct {
    unsigned addr : 24;
    unsigned len : 8;
} PTAG;
typedef struct {
    PTAG tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct {
    PTAG tag;
    int code[2];
} DRM;
typedef struct {
    short x, y;
    unsigned char u, v;
    short w, h;
    short tpage;
    short cx, cy;
} ENT;
extern ENT DAT_800778f8[];
extern void *DAT_1f800164;
extern PTAG *DAT_1f8001e0;
extern unsigned short GetClut(int x, int y);
extern void SetDrawMode(DRM *p, int dfe, int dtd, int tpage, void *tw);

#define addPrim(ot, p) (((PTAG *)(p))->addr = ((PTAG *)(ot))->addr, ((PTAG *)(ot))->addr = (unsigned)(p))

void FUN_800196bc(unsigned char c, int dfe)
{
    ENT *e = DAT_800778f8;
    SPRT *s = (SPRT *)0x1f800000;
    SPRT *p;
    SPRT *q;
    DRM *d;
    for (; e->x != -1; e++) {
        s->tag.len = 4;
        s->code = 0x64;
        s->r0 = c;
        s->g0 = c;
        s->b0 = c;
        s->x0 = e->x;
        s->y0 = e->y;
        s->u0 = e->u;
        s->v0 = e->v;
        s->w = e->w;
        s->h = e->h;
        s->clut = GetClut(e->cx, e->cy);
        p = (SPRT *)DAT_1f800164;
        *p = *s;
        q = (SPRT *)DAT_1f800164;
        d = (DRM *)(q + 1);
        DAT_1f800164 = d;
        addPrim(DAT_1f8001e0 + 1, p);
        SetDrawMode(d, dfe, 0, e->tpage, 0);
        addPrim(DAT_1f8001e0 + 1, (DRM *)(q + 1));
        DAT_1f800164 = (DRM *)DAT_1f800164 + 1;
    }
}
