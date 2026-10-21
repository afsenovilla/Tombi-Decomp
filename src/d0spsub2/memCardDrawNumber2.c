// FUNC 800ec22c 728 D0SPSUB2
// MATCHING 800ec22c 728
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
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
extern SPRT *D_1F800164;
extern unsigned long *D_1F8001E0;
extern unsigned short D_800F6D34[], D_800F6D36[], D_800F6D38[], D_800F6D3A[];
extern unsigned short GetClut(int x, int y);
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, void *tw);

void memCardDrawNumber2(int n, short x, short y, int force)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    DR_MODE *d;
    unsigned char dg[2];
    unsigned char *q;
    int i, k;
    dg[1] = n % 10;
    dg[0] = n / 10 % 10;
    for (q = dg, k = 0, i = 0; i < 2; i++, q++) {
        int cd = 0x65;
        if (i == 0 && *q == 0 && !force) continue;
        S->len = 4;
        S->code = cd;
        S->r0 = 0x80;
        S->g0 = 0x80;
        S->b0 = 0x80;
        S->x0 = x + k * 8;
        S->y0 = y;
        S->u0 = D_800F6D34[*q * 8];
        S->v0 = D_800F6D36[*q * 8];
        S->w = D_800F6D38[*q * 8];
        S->h = D_800F6D3A[*q * 8];
        k++;
        S->clut = GetClut(0x130, 0x1F0);
        p = D_1F800164;
        *p = *S;
        addPrim(D_1F8001E0 + 6, p);
        D_1F800164++;
    }
    d = (DR_MODE *)D_1F800164;
    SetDrawMode(d, 0, 0, GetTPage(0, 0, 0x40, 0), 0);
    addPrim(D_1F8001E0 + 6, d);
    D_1F800164 = (SPRT *)((DR_MODE *)D_1F800164 + 1);
}
