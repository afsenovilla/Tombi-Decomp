// VERSION SLPS_01144 dspsub2 800ED850 1260
#if defined(VERSION_SLPS_01144)
// JP memCardDrawPlayTime (PAL d0spsub2 800ebe6c, USA 800EB87C): a 4-sprite label first, every digit drawn
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
typedef struct { unsigned short u, v; } UV;
asm(".set NP_,0x1f800164");
asm(".set OT_,0x1f8001e0");
extern SPRT *NP_;
extern unsigned long *OT_;
extern UV D_800F2C74[], D_800F2C76[]; /* label uvs: u and v, as retail names them */
extern unsigned short func_8005AD6C(int x, int y); /* GetClut */
extern unsigned short func_8005AC9C(int tp, int abr, int x, int y); /* GetTPage */
extern void func_8005C954(DR_MODE *p, int dfe, int dtd, int tpage, void *tw); /* SetDrawMode */

void func_800ED850(int t, short x, short y, int bright)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    DR_MODE *d;
    unsigned char dg[5];
    unsigned char *q;
    int i, h, m;
    for (i = 0; i < 4; i++) {
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
        S->u0 = D_800F2C74[i].u;
        S->v0 = D_800F2C76[i].u;
        S->w = 8;
        S->h = 8;
        S->clut = func_8005AD6C(0x170, 0x1F0);
        p = NP_;
        *p = *S;
        addPrim(OT_ + 6, p);
        NP_++;
    }
    h = t / 216000 % 1000;
    m = t / 3600 - t / 216000 * 60;
    {
        int m10, h100, h10; unsigned char d4, d1, d2;
        h100 = h / 100; h10 = h / 10; m10 = m / 10;
        d4 = m - m10 * 10;
        d1 = h10 - h100 * 10;
        d2 = h - h10 * 10;
        dg[4] = d4;
        dg[1] = d1;
        dg[2] = d2;
        m10 %= 10;
        { short m1 = m10; dg[3] = m1; } /* short copy: fixes the digit-block registers */
        h100 %= 10;
        dg[0] = h100;
    }
    for (q = dg, i = 0; i < 6; i++) {
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
        S->x0 = x + (i + 2) * 8;
        S->code &= ~2;
        S->y0 = y;
        if (i == 3) {
            S->u0 = 0x50;
            S->v0 = 0x18;
        } else {
            S->u0 = *q++ * 8;
            S->v0 = 0x18;
        }
        S->w = 8;
        S->h = 8;
        S->clut = func_8005AD6C(0x170, 0x1F0);
        p = NP_;
        *p = *S;
        addPrim(OT_ + 6, p);
        NP_++;
    }
    d = (DR_MODE *)NP_;
    func_8005C954(d, 0, 0, func_8005AC9C(0, 0, 0x140, 0x100), 0);
    addPrim(OT_ + 6, d);
    NP_ = (SPRT *)((DR_MODE *)NP_ + 1);
}
#endif
