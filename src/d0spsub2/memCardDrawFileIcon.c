// FUNC 800eac90 684 D0SPSUB2
// MATCHING 800eac90 684
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
typedef struct { short x, y, u, v, w, h, cx, cy; } Part;
typedef struct { short x, y; } Pos;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern SPRT *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern Part D_800F6C58[], D_800F6CA8[];
extern Pos D_800F70D8;
extern short D_800F70DA, D_800F70DE;
extern short D_800F70DC[];
extern int D_800F70E8, D_800F6ABC;
extern void *D_800F70EC;
extern char D_800EDC7C[];
extern unsigned short GetClut(int x, int y);
extern void AddDrawMode(int a, int b);
extern void memCardDrawString(int a, int b);

void memCardDrawFileIcon(int big, int pri, int pal, int lit)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    Part *s;
    if (big == 0) {
        s = D_800F6C58;
    } else {
        short x, y;
        D_800F70DC[0] = 4;
        x = D_800F70D8.x;
        y = D_800F70DA;
        D_800F70D8.x = x + 0x10;
        D_800F70DA = y + 0x10;
        D_800F70DE = GetClut(0x150, 0x1F0);
        s = D_800F6CA8;
        D_800F70EC = D_800EDC7C;
        D_800F70E8 = D_800F6ABC;
        memCardDrawString(6, 0);
        D_800F70D8.x = x;
        D_800F70DA = y;
    }
    do {
        S->len = 4;
        S->code = 0x64;
        if (lit) {
            S->r0 = 0x80;
            S->g0 = 0x80;
            S->b0 = 0x80;
        } else {
            S->r0 = 0x50;
            S->g0 = 0x50;
            S->b0 = 0x50;
        }
        S->code &= ~2;
        S->x0 = D_800F70D8.x + s->x;
        S->y0 = D_800F70D8.y + s->y;
        S->u0 = s->u;
        S->v0 = s->v;
        S->w = s->w;
        S->h = s->h;
        if (s >= D_800F6CA8) S->clut = GetClut(0x140, pal + 0x1F0);
        else S->clut = GetClut(s->cx, s->cy);
        s++;
        p = NEXT_PRIM;
        *p = *S;
        addPrim(CURRENT_OT + pri + 4, p);
        NEXT_PRIM++;
    } while (s->x != -1);
    D_800F70DC[0] = 1;
    AddDrawMode(1, pri + 4);
}
