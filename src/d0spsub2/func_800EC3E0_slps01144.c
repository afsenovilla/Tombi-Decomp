// VERSION SLPS_01144 dspsub2 800EC3E0 548
#if defined(VERSION_SLPS_01144)
// JP memCardDrawBackground (PAL d0spsub2 800ea160): a counted loop over 7 parts
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
typedef struct { unsigned addr: 24; unsigned len: 8; unsigned long code[2]; } DR_MODE;
typedef struct { short x, y, u, v, w, h, pri, pad; } Part;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
asm(".set NP_,0x1f800164");
asm(".set OT_,0x1f8001e0");
extern char *NP_;
extern unsigned long *OT_;
extern Part D_800F2B04[];
extern unsigned short func_8005AD6C(int x, int y); /* GetClut */
extern unsigned short func_8005AC9C(int tp, int abr, int x, int y); /* GetTPage */
extern void func_8005C954(DR_MODE *p, int dfe, int dtd, int tpage, void *tw); /* SetDrawMode */

void func_800EC3E0(void)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    DR_MODE *m;
    int i;
    Part *s = D_800F2B04;
    for (i = 0; i < 7; i++) {
        S->len = 4;
        S->code = 0x65;
        S->r0 = 0x80;
        S->g0 = 0x80;
        S->b0 = 0x80;
        S->code &= ~2;
        S->x0 = s->x;
        S->y0 = s->y;
        S->u0 = s->u;
        S->v0 = s->v;
        S->w = s->w;
        S->h = s->h;
        S->clut = func_8005AD6C(0x80, 0x1FF);
        p = (SPRT *)NP_;
        *p = *S;
        addPrim(OT_ + 4 + s->pri, p);
        m = (DR_MODE *)(NP_ += sizeof(SPRT));
        func_8005C954(m, 0, 0, func_8005AC9C(1, 0, 0x80, 0), 0);
        addPrim(OT_ + 4 + s->pri, m);
        NP_ += sizeof(DR_MODE);
        s++;
    }
}
#endif
