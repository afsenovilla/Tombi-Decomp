// FUNC 800eb948 1316 D0SPSUB2
// MATCHING 800eb948 1316
// Memory card screen: draws a sign (two sprites from MEMCARD_SIGN_UVS) and an 8-digit number without leading zeros.
// Idioms: the digits are block-scoped temporaries (q7..q1 quotients, d1..d7 unsigned char) stored in the order
// dg[1..5], dg[7], dg[6], then `q7 %= 10; dg[0] = q7` (the 10^7 and 10^6 divisions share one mult); `q1 = n / 10`
// is written before `q2 = n / 100` so the 0x66666667 constant is loaded first; `q = dg` is set before the digit
// block (longest life -> lowest global priority -> s6); in the loop `S->u0 = *q * 8` comes right after `S->y0 = y`.
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
extern SPRT *NEXT_PRIM;
extern unsigned long *CURRENT_OT;
extern UV MEMCARD_SIGN_UVS[];
extern unsigned short GetClut(int x, int y);
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, void *tw);

void memCardDrawSignedNumber(int n, short x, short y, int bright)
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
        S->u0 = MEMCARD_SIGN_UVS[i].u;
        S->v0 = MEMCARD_SIGN_UVS[i].v;
        S->w = 8;
        S->h = 8;
        S->clut = GetClut(0x170, 0x1F0);
        p = NEXT_PRIM;
        *p = *S;
        addPrim(CURRENT_OT + 6, p);
        NEXT_PRIM++;
    }
    q = dg;
    {
        int q7, q6, q5, q4, q3, q2, q1;
        unsigned char d1, d2, d3, d4, d5, d6, d7;
        q7 = n / 10000000; q6 = n / 1000000; q5 = n / 100000; q4 = n / 10000; q3 = n / 1000; q1 = n / 10; q2 = n / 100;
        d1 = q6 - q7 * 10; d2 = q5 - q6 * 10; d3 = q4 - q5 * 10; d4 = q3 - q4 * 10; d5 = q2 - q3 * 10; d6 = q1 - q2 * 10; d7 = n - q1 * 10;
        dg[1] = d1; dg[2] = d2; dg[3] = d3; dg[4] = d4; dg[5] = d5; dg[7] = d7; dg[6] = d6;
        q7 %= 10; dg[0] = q7;
        if (dg[0] != 0 || dg[1] != 0) j = 0;
        else j = 1;
    }
    for (k = 0, i = 0; i < 8; i++, q++) {
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
        S->u0 = *q * 8;
        S->v0 = 0x18;
        S->w = 8;
        S->h = 8;
        j++;
        S->clut = GetClut(0x170, 0x1F0);
        p = NEXT_PRIM;
        *p = *S;
        addPrim(CURRENT_OT + 6, p);
        NEXT_PRIM++;
        k++;
    }
    d = (DR_MODE *)NEXT_PRIM;
    SetDrawMode(d, 0, 0, GetTPage(0, 0, 0x140, 0x100), 0);
    addPrim(CURRENT_OT + 6, d);
    NEXT_PRIM = (SPRT *)((DR_MODE *)NEXT_PRIM + 1);
}
