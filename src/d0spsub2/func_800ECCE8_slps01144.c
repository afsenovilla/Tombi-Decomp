// VERSION SLPS_01144 dspsub2 800ECCE8 348
#if defined(VERSION_SLPS_01144)
// JP memCardDrawFileCursor (PAL d0spsub2 800eaf3c): one 160x48 sprite instead of a part list
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
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned long)(_addr))
#define getaddr(p) (unsigned long)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
asm(".set NP1_,0x1f800164");
asm(".set NP2_,0x1f800164");
asm(".set NP3_,0x1f800164");
asm(".set OT_,0x1f8001e0");
/* NEXT_PRIM as three one-field structs (in-struct, so the reload stays after the OT store; one name per access) */
extern struct { SPRT *p; } NP1_, NP2_, NP3_;
extern unsigned long *OT_;
extern short D_800F3084;          /* draw tpage */
extern unsigned short func_8005AD6C(int x, int y); /* GetClut */
extern void func_8004C7BC(int a, int b); /* AddDrawMode */

void func_800ECCE8(int pri, short x, short y)
{
    SPRT *S = (SPRT *)0x1F800000;
    SPRT *p;
    S->len = 4;
    /* first code store through an int: its pseudo is SImode, so cse does not reuse it for the second (QImode)
       0x65 store; no asm barrier is needed and reorg can move `li a1,498` into the jal delay slot */
    { int k = 0x65; S->code = k; }
    S->r0 = 0x80;
    S->g0 = 0x80;
    S->b0 = 0x80;
    S->code = 0x65;
    S->x0 = x - 4;
    S->y0 = y - 4;
    S->v0 = 0x70;
    S->w = 0xA0;
    S->u0 = 0;
    S->h = 0x30;
    S->clut = func_8005AD6C(0x130, 0x1F2);
    p = NP1_.p;
    *p = *S;
    addPrim(OT_ + pri, p);
    NP3_.p = NP2_.p + 1;
    func_8004C7BC(D_800F3084, pri);
}
#endif
