// FUNC 800e9f5c 636 O0PTSUB0
// MATCHING 800e9f5c 636
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} POLY_FT4;
typedef struct { unsigned char u, v, w, h, big; } Glyph;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern Glyph D_800EA1D8[];
extern POLY_FT4 *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern int MulNegSin(int a, int b);
extern int MulCosDup(int a, int b);
extern int GetGraphType(void);

void titleDrawCarouselGlyph(short c, int ang, int off, int pal)
{
    POLY_FT4 *p;
    Glyph *tbl;
    short ch = c;
    short an = ang;
    unsigned char pl = pal;
    int a;
    if (((ang + off) & 0xFF) > 0x40 && ((ang + off) & 0xFF) < 0xC0) return;
    tbl = D_800EA1D8;
    if ((short)off == -1) {
        int r = MulNegSin(0xE0, 0x5A);
        int s = (short)ang;
        int w = tbl[(short)c].w;
        r = w * s - r;
        c = r;
        a = w + r;
    } else {
        c = -MulNegSin(ang & 0xFF, 0x5A);
        a = -MulNegSin((ang + off) & 0xFF, 0x5A);
    }
    MulCosDup(an & 0xFF, 0xFF);
    p = NEXT_PRIM;
    p->len = 9;
    p->code = 0x2C;
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->x0 = c + 0xA0;
    p->y0 = 0xB8;
    p->x1 = a + 0xA0;
    p->y1 = 0xB8;
    p->code &= ~2;
    p->x2 = p->x0;
    p->y2 = tbl[(short)ch].h + 0xB8;
    p->x3 = p->x1;
    p->y3 = p->y2;
    p->u0 = tbl[(short)ch].u;
    p->v0 = tbl[(short)ch].v;
    p->u1 = p->u0 + tbl[(short)ch].w;
    p->u2 = p->u0;
    p->v1 = p->v0;
    p->v2 = p->v0 + tbl[(short)ch].h;
    p->clut = ((pl + 0x1E0) << 6) | 0x15;
    p->u3 = p->u1;
    p->v3 = p->v2;
    p->tpage = (GetGraphType() == 1 || GetGraphType() == 2) ? 0x86 : 0x26;
    addPrim(CURRENT_OT, p);
    NEXT_PRIM++;
}
