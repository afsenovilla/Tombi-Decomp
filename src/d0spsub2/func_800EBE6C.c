// FUNC 800ebe6c 960 D0SPSUB2
// MATCHING 800ebe6c 960
// Memory card screen: draws a play time as MMM:SS (t in 1/60 s units; frame 3 is the colon sprite).
// Idiom (cf. func_800EB948): the digits are block-scoped temporaries: h100 = h/100, h10 = h/10, m10 = m/10,
// d4 = m - m10*10, d1 = h10 - h100*10, d2 = h - h10*10 (unsigned char), then `h100 %= 10`, `m10 %= 10` and the
// stores dg[4], dg[1], dg[2] last; every pseudo dies once so local-alloc ties them into the game's registers.
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
extern SPRT *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
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
        int m10, h100, h10; unsigned char d4, d1, d2;
        h100 = h / 100; h10 = h / 10; m10 = m / 10;
        d4 = m - m10 * 10;
        d1 = h10 - h100 * 10;
        d2 = h - h10 * 10;
        h100 %= 10;
        dg[0] = h100;
        m10 %= 10;
        dg[3] = m10;
        dg[4] = d4;
        dg[1] = d1;
        dg[2] = d2;
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
        p = NEXT_PRIM;
        *p = *S;
        addPrim(CURRENT_OT + 6, p);
        NEXT_PRIM++;
    }
    d = (DR_MODE *)NEXT_PRIM;
    SetDrawMode(d, 0, 0, GetTPage(0, 0, 0x140, 0x100), 0);
    addPrim(CURRENT_OT + 6, d);
    NEXT_PRIM = (SPRT *)((DR_MODE *)NEXT_PRIM + 1);
}
