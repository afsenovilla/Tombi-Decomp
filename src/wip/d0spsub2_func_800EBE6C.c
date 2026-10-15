// FUNC 800ebe6c 960 D0SPSUB2
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
extern unsigned short GetClut(int x, int y);
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, void *tw);

void func_800EBE6C(int t, short x, short y, int bright)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    DR_MODE *d;
    unsigned char dg[5];
    unsigned char *q;
    int i, k, h, m;
    h = t / 180000 % 1000;
    m = t / 3000 - t / 180000 * 60;
    dg[4] = m % 10;
    dg[1] = h / 10 - h / 100 * 10;
    dg[2] = h % 10;
    dg[3] = m / 10 % 10;
    dg[0] = h / 100 % 10;
    for (q = dg, k = 0, i = 0; i < 6; i++, q++) {
        if (i != 2 && *q == 0 && k == 0) continue;
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
        S->x0 = x + i * 8;
        S->code &= ~2;
        S->y0 = y;
        if (i == 3) {
            q--;
            S->u0 = 0x50;
            S->v0 = 0x18;
        } else {
            S->v0 = 0x18;
            S->u0 = *q * 8;
        }
        S->w = 8;
        S->h = 8;
        k++;
        S->clut = GetClut(0x170, 0x1F0);
        p = D_1F800164;
        *p = *S;
        addPrim(D_1F8001E0 + 6, p);
        D_1F800164++;
    }
    d = (DR_MODE *)D_1F800164;
    SetDrawMode(d, 0, 0, GetTPage(0, 0, 0x140, 0x100), 0);
    addPrim(D_1F8001E0 + 6, d);
    D_1F800164 = (SPRT *)((DR_MODE *)D_1F800164 + 1);
}
