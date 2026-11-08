// VERSION SLPS_01144 dspsub2 800EDD3C 696
#if defined(VERSION_SLPS_01144)
// JP memCardDrawNumber2 (PAL d0spsub2 800ec22c): no force flag, digits at fixed columns
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
asm(".set NP_,0x1f800164");
asm(".set OT_,0x1f8001e0");
extern SPRT *NP_;
extern unsigned long *OT_;
extern unsigned short D_800F2C88[], D_800F2C8A[], D_800F2C8C[], D_800F2C8E[]; /* digit sprites u, v, w, h */
extern unsigned short func_8005AD6C(int x, int y); /* GetClut */
extern unsigned short func_8005AC9C(int tp, int abr, int x, int y); /* GetTPage */
extern void func_8005C954(DR_MODE *p, int dfe, int dtd, int tpage, void *tw); /* SetDrawMode */

void func_800EDD3C(int n, short x, short y)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    DR_MODE *d;
    unsigned char dg[2];
    unsigned char *q;
    int i;
    dg[1] = n % 10;
    dg[0] = n / 10 % 10;
    for (q = dg, i = 0; i < 2; i++, q++) {
        int cd = 0x65;
        if (i == 0 && *q == 0) continue;
        S->len = 4;
        S->code = cd;
        S->r0 = 0x80;
        S->g0 = 0x80;
        S->b0 = 0x80;
        S->x0 = x + i * 8;
        S->y0 = y;
        S->u0 = D_800F2C88[*q * 8];
        S->v0 = D_800F2C8A[*q * 8];
        S->w = D_800F2C8C[*q * 8];
        S->h = D_800F2C8E[*q * 8];
        S->clut = func_8005AD6C(0x130, 0x1F0);
        p = NP_;
        *p = *S;
        addPrim(OT_ + 6, p);
        NP_++;
    }
    d = (DR_MODE *)NP_;
    func_8005C954(d, 0, 0, func_8005AC9C(0, 0, 0x40, 0), 0);
    addPrim(OT_ + 6, d);
    NP_ = (SPRT *)((DR_MODE *)NP_ + 1);
}
#endif
