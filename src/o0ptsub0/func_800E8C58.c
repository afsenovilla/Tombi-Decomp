// FUNC 800e8c58 492 O0PTSUB0
// MATCHING 800e8c58 492
typedef struct { unsigned addr: 24; unsigned len: 8; } P_TAG;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct {
    unsigned addr: 24;
    unsigned len: 8;
    unsigned long code[2];
} DR_MODE;
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern SPRT D_1F800000;
extern char *D_1F800164;
extern unsigned long *DAT_1f8001e0;
extern unsigned short GetClut(int x, int y);
extern void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, void *tw);

void func_800E8C58(SprDef *d, short tpage, unsigned char pri)
{
    SPRT *p;
    DR_MODE *m;
    D_1F800000.len = 4;
    D_1F800000.code = 0x64;
    D_1F800000.r0 = 0x80;
    D_1F800000.g0 = 0x80;
    D_1F800000.b0 = 0x80;
    D_1F800000.code |= 2;
    D_1F800000.x0 = d->x;
    D_1F800000.y0 = d->y;
    D_1F800000.u0 = d->u;
    D_1F800000.v0 = d->v;
    D_1F800000.w = d->w;
    D_1F800000.h = d->h;
    D_1F800000.clut = GetClut(d->cx, d->cy);
    p = (SPRT *)D_1F800164;
    *p = *(SPRT *)0x1F800000;
    addPrim(DAT_1f8001e0 + 4 + pri, p);
    m = (DR_MODE *)(D_1F800164 += sizeof(SPRT));
    SetDrawMode(m, 0, 0, tpage, 0);
    addPrim(DAT_1f8001e0 + 4 + pri, m);
    D_1F800164 += sizeof(DR_MODE);
}
