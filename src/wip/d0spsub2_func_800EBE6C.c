// FUNC 800ebe6c 960 D0SPSUB2
// wip: score 66 (was 68). Digit block now as block-scoped temporaries (each a block-local pseudo for local-alloc,
// cf. the matched D0SPSUB func_800EA5E8): h100 = h/100; h10 = h/10; b = h10 - h100*10; dg[2] = h - h10*10;
// m10 = m/10; dg[4] = m - m10*10; dg[1] = b; m10 %= 10; dg[3] = m10; h100 %= 10; dg[0] = h100.
// Game order of the chains: m10, dg[4] (into m's reg), h100 (a3), h100*10 (a1 chain), h10 (v0), b = h10-h100*10 (a1),
// h - h10*10 (a0 chain), then sb dg[1], sb dg[2], m10 %= 10 (a2) -> dg[3], h100 %= 10 -> dg[0].
// Left: register assignment only (game m10=a2, h100=a3, h10=v0, b-chain=a1, dg[2]-chain=a0; ours m10=a1, h100=a2,
// h10=v1, b=a0, dg[2]=v0) + the resulting schedule. All 504 dependency-respecting orders of these statements
// (block-scoped, b unsigned char) give 66 or worse; b as int 84; function-scope temps 68+.
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
    {
        int m10, h100, h10; unsigned char b;
        h100 = h / 100;
        h10 = h / 10;
        b = h10 - h100 * 10;
        dg[2] = h - h10 * 10;
        m10 = m / 10;
        dg[4] = m - m10 * 10;
        dg[1] = b;
        m10 %= 10;
        dg[3] = m10;
        h100 %= 10;
        dg[0] = h100;
    }
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
            S->u0 = *q * 8;
            S->v0 = 0x18;
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
