// VERSION SLPS_01144 dspsub2 800ED348 1288
#if defined(VERSION_SLPS_01144)
// JP memCardDrawSignedNumber (PAL d0spsub2 800eb948): digits at fixed columns, sign one column further left
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
asm(".set NP_,0x1f800164");
asm(".set OT_,0x1f8001e0");
extern SPRT *NP_;
extern unsigned long *OT_;
extern UV D_800F2C6C[], D_800F2C6E[]; /* sign uvs: u and v, as retail names them */
extern unsigned short func_8005AD6C(int x, int y); /* GetClut */
extern unsigned short func_8005AC9C(int tp, int abr, int x, int y); /* GetTPage */
extern void func_8005C954(DR_MODE *p, int dfe, int dtd, int tpage, void *tw); /* SetDrawMode */

void func_800ED348(int n, short x, short y, int bright)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    DR_MODE *d;
    unsigned char dg[8];
    unsigned char *q;
    int i, k;
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
        S->x0 = x + (i - 3) * 8;
        S->code &= ~2;
        S->y0 = y;
        S->u0 = D_800F2C6C[i].u;
        S->v0 = D_800F2C6E[i].u;
        S->w = 8;
        S->h = 8;
        S->clut = func_8005AD6C(0x170, 0x1F0);
        p = NP_;
        *p = *S;
        addPrim(OT_ + 6, p);
        NP_++;
    }
    {
        int q7, q6, q5, q4, q3, q2, q1;
        unsigned char d1, d2, d3, d4, d5, d6, d7;
        q7 = n / 10000000; q6 = n / 1000000; q5 = n / 100000; q4 = n / 10000; q1 = n / 10; q3 = n / 1000; q2 = n / 100;
        d1 = q6 - q7 * 10; d2 = q5 - q6 * 10; d3 = q4 - q5 * 10; d4 = q3 - q4 * 10; d5 = q2 - q3 * 10; d6 = q1 - q2 * 10; d7 = n - q1 * 10;
        dg[1] = d1; dg[2] = d2; dg[3] = d3; dg[4] = d4; dg[5] = d5; dg[7] = d7; dg[6] = d6;
        q7 %= 10; dg[0] = q7;
    }
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
        S->x0 = x + i * 8;
        S->code &= ~2;
        S->y0 = y;
        S->u0 = *q * 8;
        S->v0 = 0x18;
        S->w = 8;
        S->h = 8;
        S->clut = func_8005AD6C(0x170, 0x1F0);
        p = NP_;
        *p = *S;
        addPrim(OT_ + 6, p);
        NP_++;
        k++;
    }
    d = (DR_MODE *)NP_;
    func_8005C954(d, 0, 0, func_8005AC9C(0, 0, 0x140, 0x100), 0);
    addPrim(OT_ + 6, d);
    NP_ = (SPRT *)((DR_MODE *)NP_ + 1);
}
#endif
