// FUNC 800eb948 1316 D0SPSUB2
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
typedef struct { unsigned long tag; unsigned long code[2]; } DR_MODE;
typedef struct { unsigned short u, v; } UV;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern SPRT *D_1F800164;
extern unsigned long *D_1F8001E0;
extern UV D_800F6D18[];
extern unsigned short GetClut(int x, int y);
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, void *tw);

void func_800EB948(int n, short x, short y, int bright)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    DR_MODE *d;
    unsigned char dg[8];
    unsigned char *q;
    int i, j, k;
    for (i = 0; i < 2; i++) {
        S->len = 4;
        S->code = 0x64;
        if (bright) {
            S->r0 = 0x80;
            S->g0 = 0x80;
            S->b0 = 0x80;
        } else {
            S->r0 = 0x50;
            S->g0 = 0x50;
            S->b0 = 0x50;
        }
        S->x0 = x + (i - 2) * 8;
        S->code &= ~2;
        S->y0 = y;
        S->u0 = D_800F6D18[i].u;
        S->v0 = D_800F6D18[i].v;
        S->w = 8;
        S->h = 8;
        S->clut = GetClut(0x170, 0x1F0);
        p = D_1F800164;
        *p = *S;
        addPrim(D_1F8001E0 + 6, p);
        D_1F800164++;
    }
    dg[0] = n / 10000000 % 10;
    dg[1] = n / 1000000 - n / 10000000 * 10;
    dg[2] = n / 100000 - n / 1000000 * 10;
    dg[3] = n / 10000 - n / 100000 * 10;
    dg[4] = n / 1000 - n / 10000 * 10;
    dg[5] = n / 100 - n / 1000 * 10;
    dg[6] = n / 10 - n / 100 * 10;
    dg[7] = n % 10;
    if (dg[0] != 0 || dg[1] != 0) j = 0;
    else j = 1;
    for (q = dg, k = 0, i = 0; i < 8; i++, q++) {
        if (i != 7 && *q == 0 && k == 0) continue;
        S->len = 4;
        S->code = 0x64;
        if (bright) {
            S->r0 = 0x80;
            S->g0 = 0x80;
            S->b0 = 0x80;
        } else {
            S->r0 = 0x50;
            S->g0 = 0x50;
            S->b0 = 0x50;
        }
        S->x0 = x + j * 8;
        S->code &= ~2;
        S->y0 = y;
        S->v0 = 0x18;
        S->w = 8;
        S->h = 8;
        S->u0 = *q * 8;
        j++;
        S->clut = GetClut(0x170, 0x1F0);
        p = D_1F800164;
        *p = *S;
        addPrim(D_1F8001E0 + 6, p);
        D_1F800164++;
        k++;
    }
    d = (DR_MODE *)D_1F800164;
    SetDrawMode(d, 0, 0, GetTPage(0, 0, 0x140, 0x100), 0);
    addPrim(D_1F8001E0 + 6, d);
    D_1F800164 = (SPRT *)((DR_MODE *)D_1F800164 + 1);
}
