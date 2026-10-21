// FUNC 800f0930 300 D0SPSUB
// MATCHING 800f0930 300
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
typedef struct { short x, y; } DVECTOR;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern char *D_1F800164;
extern unsigned long *D_1F8001E0;

void subScreenMapDrawNode(int pos, int pri)
{
    POLY_FT4 *p = (POLY_FT4 *)D_1F800164;
    p->len = 9;
    p->code = 0x2C;
    p->x0 = pos - 4;
    p->y0 = (pos >> 16) - 4;
    p->u0 = 0x70;
    p->v0 = 0x90;
    p->x1 = p->x0 + 8;
    p->y2 = p->y0 + 8;
    p->y1 = p->y0;
    p->x2 = p->x0;
    p->u1 = p->u0 + 8;
    p->v2 = p->v0 + 8;
    p->r0 = 0xFF;
    p->g0 = 0xFF;
    p->b0 = 0xFF;
    p->x3 = p->x1;
    p->y3 = p->y2;
    p->v1 = p->v0;
    p->u2 = p->u0;
    p->u3 = p->u1;
    p->v3 = p->v2;
    p->tpage = 0x85;
    p->clut = 0x7FC8;
    addPrim(D_1F8001E0 - 5 + pri, p);
    D_1F800164 += sizeof(POLY_FT4);
}
