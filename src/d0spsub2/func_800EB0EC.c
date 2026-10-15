// FUNC 800eb0ec 368 D0SPSUB2
// MATCHING 800eb0ec 368
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
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define S ((SPRT *)0x1F800000)
extern SPRT *D_1F800164;
extern unsigned long *D_1F8001E0;
extern short D_800F70DC[];
extern unsigned short GetClut(int x, int y);
extern void AddDrawMode(int a, int b);

void func_800EB0EC(int no, int pri, short x, short y)
{
    SPRT *p;
    SPRT *s = (SPRT *)0x1F800000;
    S->len = 4;
    S->code = 0x64;
    S->code |= 1;
    S->r0 = 0x80;
    S->g0 = 0x80;
    S->b0 = 0x80;
    S->code &= ~2;
    S->x0 = x;
    S->y0 = y;
    if (no) {
        S->u0 = 0xA8;
        S->v0 = 0x78;
    } else {
        S->u0 = 0xA8;
        S->v0 = 0x88;
    }
    s->w = 0x28;
    s->h = 0x10;
    s->clut = GetClut(0x130, 0x1F3);
    p = D_1F800164;
    *p = *s;
    addPrim(D_1F8001E0 + pri, p);
    D_1F800164++;
    AddDrawMode(D_800F70DC[0], pri);
}
