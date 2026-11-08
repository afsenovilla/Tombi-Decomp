// VERSION SLPS_01144 dspsub2 800ECA44 676
#if defined(VERSION_SLPS_01144)
// JP memCardDrawFileIcon (PAL d0spsub2 800eac90): the palette clut goes to the parts at x 0
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
typedef struct { short x, y, u, v, w, h, cx, cy; } Part;
typedef struct { short x, y; } Pos;
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
asm(".set NP_,0x1f800164");
asm(".set OT_,0x1f8001e0");
extern SPRT *NP_;
extern unsigned long *OT_;
extern Part D_800F2B74[], D_800F2BC4[]; /* file icon parts, empty slot parts */
extern Pos D_800F3080;            /* draw params x, y */
extern short D_800F3082, D_800F3086; /* draw y, draw clut */
extern short D_800F3084[];        /* draw tpage */
extern int D_800F3090, D_800F2AFC; /* draw string, empty slot string */
extern void *D_800F3094;          /* draw font */
extern char D_800EEE54[];         /* font */
extern unsigned short func_8005AD6C(int x, int y); /* GetClut */
extern void func_8004C7BC(int a, int b); /* AddDrawMode */
extern void func_800ED0E0(int a); /* memCardDrawString */

void func_800ECA44(int big, int pri, int pal, int lit)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    Part *s;
    if (big == 0) {
        s = D_800F2B74;
    } else {
        short x, y;
        D_800F3084[0] = 4;
        x = D_800F3080.x;
        y = D_800F3082;
        D_800F3080.x = x + 0x10;
        D_800F3082 = y + 0x10;
        D_800F3086 = func_8005AD6C(0x150, 0x1F0);
        s = D_800F2BC4;
        D_800F3094 = D_800EEE54;
        D_800F3090 = D_800F2AFC;
        func_800ED0E0(6);
        D_800F3080.x = x;
        D_800F3082 = y;
    }
    do {
        S->len = 4;
        S->code = 0x64;
        if (lit) {
            S->r0 = 0x80;
            S->g0 = 0x80;
            S->b0 = 0x80;
        } else {
            S->r0 = 0x50;
            S->g0 = 0x50;
            S->b0 = 0x50;
        }
        S->code &= ~2;
        S->x0 = D_800F3080.x + s->x;
        S->y0 = D_800F3080.y + s->y;
        S->u0 = s->u;
        S->v0 = s->v;
        S->w = s->w;
        S->h = s->h;
        if (s->x == 0) S->clut = func_8005AD6C(0x140, pal + 0x1F0);
        else S->clut = func_8005AD6C(s->cx, s->cy);
        s++;
        p = NP_;
        *p = *S;
        addPrim(OT_ + pri + 4, p);
        NP_++;
    } while (s->x != -1);
    D_800F3084[0] = 1;
    func_8004C7BC(1, pri + 4);
}
#endif
