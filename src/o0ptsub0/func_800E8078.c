// FUNC 800e8078 516 O0PTSUB0
// MATCHING 800e8078 516
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
typedef struct { short x, y; unsigned char u, v; short w, h, tp, cx, cy; } SprRec;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern char *D_1F800164;
extern unsigned long *DAT_1f8001e0;
extern unsigned short GetClut(int x, int y);
extern void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, void *tw);

void func_800E8078(SprRec *r, int x, short y, int dfe)
{
    SPRT *p;
    DR_MODE *m;
    SPRT *SP = (SPRT *)0x1F800000;
    while (r->x != -1) {
        int cd = 0x65;
        SP->len = 4;
        SP->code = cd;
        SP->r0 = 0x80;
        SP->g0 = 0x80;
        SP->b0 = 0x80;
        SP->x0 = r->x + x;
        SP->y0 = r->y + y;
        SP->u0 = r->u;
        SP->v0 = r->v;
        SP->w = r->w;
        SP->h = r->h;
        SP->clut = GetClut(r->cx, r->cy);
        p = (SPRT *)D_1F800164;
        *p = *SP;
        addPrim(DAT_1f8001e0 + 1, p);
        m = (DR_MODE *)(D_1F800164 += sizeof(SPRT));
        SetDrawMode(m, dfe, 0, r->tp, 0);
        addPrim(DAT_1f8001e0 + 1, m);
        D_1F800164 += sizeof(DR_MODE);
        r++;
    }
}
