// FUNC 80118028 208 X013
// MATCHING 80118028 208
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u_long;
typedef struct { unsigned addr : 24; unsigned len : 8; u8 r0, g0, b0, code; } P_TAG;
typedef struct { u_long tag; u8 r0, g0, b0, code; short x0, y0; u8 u0, v0; u16 clut; short w, h; } SPRT;
typedef struct { u16 u, v; } UV;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (u_long)(_addr))
#define getaddr(p) (u_long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define setlen(p, _len) (((P_TAG *)(p))->len = (u8)(_len))
#define setcode(p, _code) (((P_TAG *)(p))->code = (u8)(_code))
extern SPRT *D_1F800164;
extern u_long D_1F8001E0;
extern UV D_80118104[];

void func_80118028(int i, short x, short y)
{
    SPRT *p;

    p = D_1F800164;
    setlen(p, 4);
    setcode(p, 0x65);
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->x0 = x;
    p->y0 = y;
    p->code &= ~2;
    p->u0 = D_80118104[i].u;
    p->v0 = D_80118104[i].v;
    p->w = 8;
    p->h = 0x10;
    p->clut = 0x7812;
    addPrim(D_1F8001E0 + 0x14, p);
    D_1F800164 = (SPRT *)((char *)D_1F800164 + 0x14);
}
